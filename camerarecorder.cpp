#include "CameraRecorder.h"
#include "cameracontroller.h"
#include <QDir>
#include <QDebug>

CameraRecorder::CameraRecorder(CameraController *cam)
    : m_cam(cam)
{
}

CameraRecorder::~CameraRecorder()
{
}

bool CameraRecorder::prepare(const QString &rootDir)
{
    QString folder = rootDir + "/Camera";
    QDir().mkpath(folder);
    m_saveDir = folder;
    return true;
}

bool CameraRecorder::start()
{
    m_frameId = 0;
    return true;
}

void CameraRecorder::stop()
{
}

void CameraRecorder::recordFrame(double timestampMs)
{
    QString fname =
        QString("%1/frame_%2.npy")
            .arg(m_saveDir)
            .arg(m_frameId++, 6, 10, QLatin1Char('0'));

    m_cam->saveLastPointCloudNpy(fname.toStdString());
}
