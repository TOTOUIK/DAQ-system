#pragma once

#include <QObject>
#include <QThread>
#include <QVector>
#include <QStringList>
#include <QMutex>
#include <atomic>

#include "NIDAQmx.h"

class AcquisitionWorker;

class NIDaqController : public QObject
{
    Q_OBJECT
public:
    explicit NIDaqController(QObject *parent = nullptr);
    ~NIDaqController() override;

    bool init();        // 识别硬件并打印
    bool configure(int slot1, int slot2);  // 配置某个槽位的4通道（比如 slot=1 → cDAQ1Mod1)
    void setSampleRate(double sampleRateHz);

    bool start();
    void stop();
    bool isRunning() const;
    bool startDual(int slot1, int slot2);
    void stopDual();
signals:
    void newSamples1(const QVector<double> &samples, double timestamp);
    void newSamples2(const QVector<double> &samples, double timestamp);
    void errorOccurred(const QString &msg);

private:
    // 两个线程与 worker
    QThread *thread1 = nullptr;
    QThread *thread2 = nullptr;

    AcquisitionWorker *worker1 = nullptr;
    AcquisitionWorker *worker2 = nullptr;

    QString m_deviceChassis;           // 如 "cDAQ1"
    QStringList m_availableModules;    // 如 ["cDAQ1Mod1", "cDAQ1Mod2"]
    QString m_selectedModule1;          // 当前采集的模块，如 "cDAQ1Mod1"
    QString m_selectedModule2;

    double m_sampleRate = 1000.0;
    std::atomic<bool> m_running {false};

    friend class AcquisitionWorker;
};

