#include "camerarecorder.h"
#include "cameracontroller.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>

CameraRecorder::CameraRecorder(CameraController *cam)
    : m_cam(cam)
{
}

bool CameraRecorder::prepare(const QString &rootDir)
{
    QString folder = rootDir;
    if (!folder.endsWith('/')) folder += '/';
    folder += "Camera";
    if (!QDir().mkpath(folder)) {
        qWarning() << "Failed to create camera save directory:" << folder;
        return false;
    }

    QMutexLocker locker(&mutex);
    m_saveDir = folder;
    frames.clear();
    return true;
}

void CameraRecorder::append(double timestamp)
{
    if (!m_cam) return;
    if (m_saveDir.isEmpty()) return;

    // 生成文件名：frame_000001.npy
    QMutexLocker locker(&mutex);
    QString fname = QString("%1/frame_%2.npy")
                        .arg(m_saveDir)
                        .arg(frames.size(), 6, 10, QLatin1Char('0'));

    // 尝试让 CameraController 保存当前点云为 npy
    // CameraController::saveLastPointCloudNpy 接受 std::string（代码中已有用法）
    try {
        m_cam->saveLastPointCloudNpy(fname.toStdString());
    } catch (...) {
        qWarning() << "CameraRecorder: exception while saving point cloud to" << fname;
        return;
    }

    FrameEntry e;
    e.filename = fname;
    e.timestamp = timestamp;
    frames.append(e);
}

bool CameraRecorder::saveToFile(const QString &filePath)
{
    QMutexLocker locker(&mutex);

    if (frames.isEmpty()) return false;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "CameraRecorder: cannot open index file for writing:" << filePath;
        return false;
    }

    QTextStream out(&file);
    out << "frame_id,filename,time_s\n";
    for (int i = 0; i < frames.size(); ++i) {
        const auto &f = frames.at(i);
        out << i << "," << "\"" << f.filename << "\"" << "," << f.timestamp << "\n";
    }
    return true;
}

void CameraRecorder::clear()
{
    QMutexLocker locker(&mutex);
    frames.clear();
}

bool CameraRecorder::hasData() const
{
    QMutexLocker locker(&mutex);
    return !frames.isEmpty();
}
