#pragma once

#include <QObject>
#include <QStringList>
#include <atomic>
#include <QMutex>

class NIDaqController : public QObject
{
    Q_OBJECT
public:
    explicit NIDaqController(QObject *parent = nullptr);
    ~NIDaqController() override;

    // 初始化，识别设备并打印信息
    bool init();

    // --- 后续会用到的接口 ---
    bool configureChannels(const QStringList &physicalChannels, bool flattenChannels = true);
    void setSampleRate(double sampleRateHz);
    bool start();
    void stop();
    bool isRunning() const;
    QVector<double> filterSamples(const QVector<double> &samples);

signals:
    void errorOccurred(const QString &msg);
    void started();
    void stopped();

private:
    std::atomic<bool> m_running {false};
    QStringList m_physicalChannelsExpanded;
    double m_sampleRateHz = 1000.0;
    QMutex m_mutex;

    QStringList expandPhysicalChannels(const QStringList &channels);
};
