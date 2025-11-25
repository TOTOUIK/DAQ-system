#pragma once
#include "DataRecorderBase.h"
#include <QFile>
#include <QTextStream>
#include <QVector>

// class VoltageRecorder : public DataRecorderBase
// {
// public:
//     VoltageRecorder() = default;
//     ~VoltageRecorder();

//     bool prepare(const QString& rootDir) override;
//     bool start() override;
//     void stop() override;

//     // NI 一帧数据：samples = 4 电压值
//     void setLatestSamples(const QVector<double>& s);
//     void recordFrame(double timestampMs) override;

// private:
//     QFile m_file;
//     QTextStream m_ts;

//     QVector<double> m_lastSamples{0,0,0,0};
//     bool m_hasSamples = false;
// };
#pragma once
#include "datarecorderbase.h"
#include <QVector>
#include <QMutex>

class VoltageRecorder : public DataRecorderBase
{
public:
    // 记录一帧：4 通道 + 时间戳
    void append(const QVector<double> &samples, double timestamp);

    bool saveToFile(const QString &filePath) override;
    void clear() override;
    bool hasData() const override;

private:
    struct SampleFrame {
        double timestamp;        // 秒
        QVector<double> values;  // 4 通道
    };

    QVector<SampleFrame> frames;
    mutable QMutex mutex;
};
