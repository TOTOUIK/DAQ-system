#pragma once

#include <QObject>
#include <atomic>
#include <QImage>
#include "cameracontroller.h"
#include "camerarecorder.h"

class CameraScanWorker : public QObject
{
    Q_OBJECT
public:
    explicit CameraScanWorker(CameraController *cam,
                              CameraRecorder *rec,
                              double intervalSeconds = 1.0,
                              QObject *parent = nullptr);

public slots:
    void process();
    void stop();

signals:
    void frameReady(const QImage &img); // 用于 UI 预览
    void finished();

private:
    CameraController *m_cam = nullptr;
    CameraRecorder *m_rec = nullptr;
    double m_intervalSeconds = 1.0;   //连续扫描间隔
    std::atomic<bool> m_running { true };
};
