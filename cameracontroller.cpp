#include "cameracontroller.h"
#include <QDebug>
#include <fstream>
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


bool CameraController::captureDepth(QImage& outDepthImg, GC3DMetaData& outMeta)
{   qDebug()<<"准备采集深度图";
    QMutexLocker locker(&m_mutex);
    if (!m_device) return false;

    if (m_device->snapShot3D() != GC3D_SUCCESS)
        return false;
    qDebug()<<"采集深度图";
    if (m_device->getGC3DMetaData(outMeta) != GC3D_SUCCESS)
        return false;

    int W = outMeta.imgW;
    int H = outMeta.imgH;

    // depthImageData → 伪彩显示
    cv::Mat gray(H, W, CV_8UC1, outMeta.depthImageData);
    cv::Mat color;
    cv::applyColorMap(gray, color, cv::COLORMAP_JET);

    outDepthImg = QImage(color.data, W, H, color.step, QImage::Format_BGR888).copy();
    qDebug() << "深度图采集成功";
    return true;
}

// 保存最近一次 meta
void CameraController::setLastMeta(const gc3d::GC3DMetaData& meta)
{
    QMutexLocker locker(&m_mutex);
    m_lastMeta = meta;        // 结构体复制 OK（SDK 内部用指针也没问题）
    m_lastMetaValid = true;
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
