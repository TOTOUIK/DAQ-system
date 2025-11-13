#include "forcemodule.h"
#include "syncmanager.h"
#include "SystemConfig.h"
#include <QRandomGenerator>
#include <QDebug>
#include <QTimer>

ForceModule::ForceModule(QObject *parent)
    : QObject(parent), running(false)
{
    this->moveToThread(&workerThread);

    connect(&workerThread, &QThread::started, [this]() {
        running = true;
        QTimer *timer = new QTimer();
        timer->setInterval(SystemConfig::SAMPLE_INTERVAL_MS);
        connect(timer, &QTimer::timeout, this, [this]() {
            if (!running) return;

            double fx = QRandomGenerator::global()->generateDouble() * 20.0 - 10.0;
            double fy = QRandomGenerator::global()->generateDouble() * 20.0 - 10.0;
            double fz = QRandomGenerator::global()->generateDouble() * 20.0 - 10.0;
            double timestamp = SyncManager::instance().nowMs();

            emit newForceData(timestamp, fx, fy, fz);
            qDebug() << "三分力 - 时间:" << timestamp << "ms, Fx=" << fx << " Fy=" << fy << " Fz=" << fz;
        });
        timer->start();
    });

    connect(&workerThread, &QThread::finished, &workerThread, &QObject::deleteLater);
}

ForceModule::~ForceModule()
{
    stop();
    workerThread.quit();
    workerThread.wait();
}

void ForceModule::start()
{
    if (!workerThread.isRunning())
        workerThread.start();
}

void ForceModule::stop()
{
    running = false;
}
