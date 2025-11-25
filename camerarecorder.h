#pragma once
#include "DataRecorderBase.h"
#include <QString>
#include <QVector>

class CameraController;

class CameraRecorder : public DataRecorderBase
{
public:
    CameraRecorder(CameraController* cam);
    ~CameraRecorder();

    bool prepare(const QString& rootDir) override;
    bool start() override;
    void stop() override;

    void recordFrame(double timestampMs) override;

private:
    CameraController* m_cam;
    int m_frameId = 0;
    QString m_saveDir = "";
};
