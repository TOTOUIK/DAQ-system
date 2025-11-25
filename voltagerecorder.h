#pragma once
#include "datarecorderbase.h"
#include <QVector>
#include <QMutex>

    class VoltageRecorder : public DataRecorderBase
{
public:
    // 记录一帧：4 通道 + 时间戳（秒）
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
