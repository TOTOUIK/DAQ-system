#include "cameracontroller.h"
#include <QDebug>

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


