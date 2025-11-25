#pragma once

#include <QObject>
#include <QElapsedTimer>
#include <atomic>
#include "NIDAQmx.h"

class AcquisitionWorker : public QObject
{
    Q_OBJECT
public:
    explicit AcquisitionWorker(const QString &physicalDevice, double sampleRate);
    ~AcquisitionWorker();

public slots:
    void process();
    void stop();

signals:
    void newSamples(const QVector<double> &samples, double timestamp);
    void finished();

private:
    QString m_dev;
    double m_rate;
    std::atomic<bool> m_running {true};

    TaskHandle m_task = 0;
};
