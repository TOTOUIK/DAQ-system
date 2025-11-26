#include "cameracontroller.h"
#include <QDebug>
#include <fstream>
#include <QElapsedTimer>
#include <QThread>
using namespace gc3d;

CameraController::CameraController(QObject* parent)
    : QObject(parent)
{
}

CameraController::~CameraController()
{
    closeCamera();
}

bool CameraController::initCamera()
{
    QMutexLocker locker(&m_mutex);

    m_device = new GC3DDevice();
    if (m_device->initialDevice() != GC3D_SUCCESS) {
        emit cameraError("相机初始化失败！");
        delete m_device;
        m_device = nullptr;
        return false;
    }

    qDebug() << "相机初始化成功";
    return true;
}

void CameraController::closeCamera()
{
    QMutexLocker locker(&m_mutex);
    if (m_device) {
        m_device->closeDevice();
        delete m_device;
        m_device = nullptr;
        qDebug() << "相机已关闭";
    }
}

void CameraController::applyParameters(
    int exposureMs,
    int smooth,
    int thMin, int thMax,
    int heightMin, int heightMax,
    float denoise1, float denoise2)
{
    QMutexLocker locker(&m_mutex);
    if (!m_device) return;

    // ---- 设置曝光 ----
    m_exposureUs = exposureMs * 1000;
    GC3DCameraParameters params = m_device->getCameraParameters();
    params.enableGamma = false;
    params.exposureNum = 1;
    params.gain = 0;
    params.exposureTime = m_exposureUs  ;
    m_device->setCameraParameters(params);
    // === 平滑参数 ===
    m_device->setSmoothParam(smooth);

    // === 重建阈值 ===
    m_device->setReconThreshold(thMin, thMax);

    // === 高度范围 ===
    // m_device->setHeightRange(420.0f + heightMin, 420.0f + heightMax);
    m_device->setHeightRange(heightMin,heightMax);
    m_device->setNeedGridData(false);
    // === 降噪参数 ===
    int fmr = 3;                 // 默认去噪半径
    float denoiseIndex1 = denoise1; // 基于邻域有效点数量（0.1-5）
    float denoiseIndex2 = denoise2; // 基于邻域点距（0-50）
    float denoiseIndex3 = 20;     // 默认局部降噪参数
    m_device->setDenoiseParameters(fmr, denoiseIndex1, denoiseIndex2, denoiseIndex3);
}


bool CameraController::capturePreview(QImage& outImg)
{
    QMutexLocker locker(&m_mutex);
    if (!m_device) return false;

    DeviceInformation info;
    m_device->getDeviceInfo(info);
    int W = info.sensorWidth;
    int H = info.sensorHeight;
    std::vector<unsigned char> buf(W * H);
    qDebug() << "曝光时长"<<m_exposureUs<<"ms";
    // 拍摄灰度预览图
    m_device->snapShot2D(m_exposureUs, 0.0, buf.data());
    outImg = QImage(buf.data(), W, H, QImage::Format_Grayscale8).copy();

    qDebug() << "预览图采集成功";
    return true;
}


// 替换现有 captureDepth 实现，新增重试与详细日志


bool CameraController::captureDepth(QImage& outDepthImg, GC3DMetaData& outMeta)
{
    QMutexLocker locker(&m_mutex);
    if (!m_device) {
        qDebug() << "[Camera] captureDepth: device == nullptr";
        return false;
    }

    qDebug() << "[Camera] captureDepth: calling snapShot3D()";
    if (m_device->snapShot3D() != GC3D_SUCCESS) {
        emit cameraError("3D扫描失败！");
        qDebug() << "[Camera][ERR] snapShot3D failed";
        return false;
    }

    if (m_device->getGC3DMetaData(outMeta) != GC3D_SUCCESS) {
        emit cameraError("获取3D数据失败！");
        qDebug() << "[Camera][ERR] getGC3DMetaData failed";
        return false;
    }

    int W = outMeta.imgW;
    int H = outMeta.imgH;
    if (W <= 0 || H <= 0 || !outMeta.depthImageData) {
        emit cameraError("深度图数据为空或尺寸非法");
        qDebug() << "[Camera][ERR] depth data invalid W,H,ptr =" << W << H << (void*)outMeta.depthImageData;
        return false;
    }

    // 生成伪彩图并把无效点设为黑色（与现有实现一致）
    cv::Mat gray(H, W, CV_8UC1, outMeta.depthImageData);
    cv::Mat color;
    cv::applyColorMap(gray, color, cv::COLORMAP_JET);

    cv::Mat black_bg = cv::Mat::zeros(H, W, CV_8UC3);
    if (outMeta.maskflag) {
        for (int i = 0; i < H; ++i) {
            for (int j = 0; j < W; ++j) {
                int idx = i * W + j;
                if (outMeta.maskflag[idx]) {
                    black_bg.at<cv::Vec3b>(i, j) = color.at<cv::Vec3b>(i, j);
                }
            }
        }
    } else {
        // 没有 maskflag 则直接使用 color
        black_bg = color;
    }

    QImage tmp(black_bg.data, W, H, static_cast<int>(black_bg.step), QImage::Format_BGR888);
    outDepthImg = tmp.copy();
    if (outDepthImg.isNull()) {
        qDebug() << "[Camera][ERR] outDepthImg is null after copy";
        return false;
    }

    qDebug() << "[Camera] captureDepth OK, img W,H =" << outDepthImg.width() << outDepthImg.height();
    return true;
}



// 保存最近一次 meta
void CameraController::setLastMeta(const gc3d::GC3DMetaData& meta)
{
    QMutexLocker locker(&m_mutex);
    m_lastMeta = meta;        // 结构体复制 OK（SDK 内部用指针也没问题）
    m_lastMetaValid = true;
}

const gc3d::GC3DMetaData* CameraController::getLastMeta() const
{
    return m_lastMetaValid ? &m_lastMeta : nullptr;
}

bool CameraController::saveLastPointCloudNpy(const std::string &filepath) {
    if (!m_lastMetaValid) return false;
    const gc3d::GC3DMetaData &meta = m_lastMeta; // 假定你在类里保存了 lastMeta 成员

    if (!meta.x || !meta.y || !meta.z || !meta.maskflag) return false;

    int W = meta.imgW;
    int H = meta.imgH;
    std::vector<float> data; // will store x,y,z sequentially
    data.reserve(W * H * 3);

    for (int i = 0; i < W * H; ++i) {
        if (!meta.maskflag[i]) continue;
        data.push_back(meta.x[i]);
        data.push_back(meta.y[i]);
        data.push_back(meta.z[i]);
    }
    size_t rows = data.size() / 3;
    if (rows == 0) return false;

    // build numpy .npy header (version 1.0)
    std::ofstream ofs(filepath, std::ios::binary);
    if (!ofs) return false;

    // magic string
    ofs.write("\x93NUMPY", 6);
    // version 1.0
    unsigned char ver[2] = {1, 0};
    ofs.write(reinterpret_cast<char*>(ver), 2);

    // dtype '<f4' (little-endian float32), fortran_order False, shape (rows,3)
    std::ostringstream header;
    header << "{'descr': '<f4', 'fortran_order': False, 'shape': (" << rows << ", 3), }";
    std::string h = header.str();
    // pad to 16-byte alignment after 10 byte prefix (magic+ver+len)
    int pad = 16 - ((10 + (int)h.size()) % 16);
    if (pad == 16) pad = 0;
    h.append(pad, ' ');
    uint16_t hlen = static_cast<uint16_t>(h.size());
    ofs.write(reinterpret_cast<char*>(&hlen), 2); // little endian
    ofs.write(h.c_str(), h.size());

    // write binary data as float32
    ofs.write(reinterpret_cast<const char*>(data.data()), data.size() * sizeof(float));
    ofs.close();
    return true;
}
