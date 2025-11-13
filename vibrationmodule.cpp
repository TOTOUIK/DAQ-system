#include "vibrationmodule.h"
#include "syncmanager.h"
#include "SystemConfig.h"
#include <QRandomGenerator>
#include <QDebug>
#include <QtMath>
#include <QTimer>

VibrationModule::VibrationModule(QObject *parent)
    : QObject(parent), running(false)
{
    this->moveToThread(&workerThread);

    connect(&workerThread, &QThread::started, [this]() {

        running = true;
        const double freq = 10.0;

        QTimer *timer = new QTimer();
        timer->setInterval(SystemConfig::SAMPLE_INTERVAL_MS);
        timer->moveToThread(&workerThread);

        connect(timer, &QTimer::timeout, this, [this, freq]() {
            if (!running) return;

            double vibration = qSin(2 * M_PI * freq * timeSec)
                               + QRandomGenerator::global()->generateDouble() * 0.1;

            double timestamp = SyncManager::instance().nowMs();
            emit newVibrationData(timestamp, vibration);

            qDebug() << "振动信号:" << vibration;

            timeSec += SystemConfig::SAMPLE_INTERVAL_MS / 1000.0;
        });

        timer->start();
    });

    connect(&workerThread, &QThread::finished, &workerThread, &QObject::deleteLater);
}

VibrationModule::~VibrationModule()
{
    stop();
    workerThread.quit();
    workerThread.wait();
}

void VibrationModule::start()
{
    if (!workerThread.isRunning())
        workerThread.start();
}

void VibrationModule::stop()
{
    running = false;
}
