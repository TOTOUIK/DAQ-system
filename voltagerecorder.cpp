#include "voltagerecorder.h"
#include <QFile>
#include <QTextStream>
#include <QMutexLocker>

    void VoltageRecorder::append(const QVector<double> &samples, double timestamp)
{
    if (samples.size() != 4) return;

    QMutexLocker locker(&mutex);

    SampleFrame f;
    f.timestamp = timestamp;
    f.values = samples;

    frames.append(f);
}

bool VoltageRecorder::saveToFile(const QString &filePath)
{
    QMutexLocker locker(&mutex);

    if (frames.isEmpty())
        return false;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);

    // CSV header
    out << "time_s,ai0,ai1,ai2,ai3\n";

    for (const auto &f : frames)
    {
        out << f.timestamp << ","
            << f.values[0] << ","
            << f.values[1] << ","
            << f.values[2] << ","
            << f.values[3] << "\n";
    }

    return true;
}

void VoltageRecorder::clear()
{
    QMutexLocker locker(&mutex);
    frames.clear();
}

bool VoltageRecorder::hasData() const
{
    QMutexLocker locker(&mutex);
    return !frames.isEmpty();
}
