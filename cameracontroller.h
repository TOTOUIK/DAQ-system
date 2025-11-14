#pragma once
#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <gc3d.h>
#include <gc3ddef.h>

class CameraController : public QObject
{
    Q_OBJECT
public:
    explicit CameraController(QObject* parent = nullptr);
    ~CameraController();

    bool initCamera();   // 初始化
    void closeCamera();  // 关闭

    bool capturePreview(QImage& outImg);
    bool captureDepth(QImage& outDepthImg,
                      gc3d::GC3DMetaData& outMeta);
    void applyParameters(int exposureMs,
                         int smooth,
                         int thMin, int thMax,
                         int heightMin, int heightMax,
                         float denoise1, float denoise2);
signals:
    void cameraError(const QString& msg);

private:
    QMutex m_mutex;
    gc3d::GC3DDevice* m_device = nullptr;
    // ---- 相机参数缓存 ----
    int m_exposureUs = 5000;   // 默认 5ms
    int m_smooth = 5;
    int m_thMin = 0, m_thMax = 255;
    int m_heightMin = -50, m_heightMax = 50;
    float m_dn1 = 0.0f, m_dn2 = 7.0f;
};
