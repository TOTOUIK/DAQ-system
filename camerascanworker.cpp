#include "camerascanworker.h"
#include "syncmanager.h"
#include <QThread>
#include <QDebug>
#include <gc3d.h>

CameraScanWorker::CameraScanWorker(CameraController *cam,
                                   CameraRecorder *rec,
                                   double intervalSeconds,
                                   QObject *parent)
    : QObject(parent),
    m_cam(cam),
    m_rec(rec),
    m_intervalSeconds(intervalSeconds)
{
}

void CameraScanWorker::process()
{
    m_running.store(true);
    while (m_running.load())
    {
        if (!m_cam) break;

        QImage depthImg;
        gc3d::GC3DMetaData meta;

        // captureDepth 内部使用互斥，线程安全的调用
        bool ok = m_cam->captureDepth(depthImg, meta);
        if (ok)
        {
            // 记录 meta 并让 CameraRecorder 立即保存点云（CameraRecorder::append 会调用 CameraController::saveLastPointCloudNpy）
            m_cam->setLastMeta(meta);

            double ts_s = SyncManager::instance().nowMs() * 0.001;
            if (m_rec) {
                m_rec->append(ts_s);
            }

            // 发出用于 UI 显示的信号（副本）
            emit frameReady(depthImg);
        }
        else
        {
            qDebug() << "[CameraScanWorker] captureDepth failed";
        }

        // sleep for the configured interval but wake up promptly on stop()
        int ms = static_cast<int>(m_intervalSeconds * 1000.0);
        for (int slept = 0; m_running.load() && slept < ms; slept += 50)
            QThread::msleep(50);
    }

    emit finished();
}

void CameraScanWorker::stop()
{
    m_running.store(false);
}
