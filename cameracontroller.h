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

    bool capturePreview(QImage& outImg, int exposureMs);
    bool captureDepth(QImage& outDepthImg,
                      gc3d::GC3DMetaData& outMeta,
                      int exposureMs,
                      int thMin, int thMax,
                      int smoothMin, int smoothMax,
                      float denoise1, float denoise2);

signals:
    void cameraError(const QString& msg);

private:
    void applyParameters(int exposureMs,
                         int thMin, int thMax,
                         int smoothMin, int smoothMax,
                         float denoise1, float denoise2);

    QMutex m_mutex;
    gc3d::GC3DDevice* m_device = nullptr;
};
