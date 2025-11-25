#include "VoltageRecorder.h"
#include <QDir>
#include <QDebug>

// VoltageRecorder::~VoltageRecorder()
// {
//     stop();
// }

// bool VoltageRecorder::prepare(const QString &rootDir)
// {
//     QString folder = rootDir + "/NI";
//     QDir().mkpath(folder);

//     m_filename = folder + "/voltage_" +
//                  QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".csv";

//     return true;
// }

// bool VoltageRecorder::start()
// {
//     m_file.setFileName(m_filename);
//     if (!m_file.open(QIODevice::WriteOnly | QIODevice::Text))
//         return false;

//     m_ts.setDevice(&m_file);
//     m_ts << "timestamp_ms, ai0, ai1, ai2, ai3\n";
//     return true;
// }

// void VoltageRecorder::stop()
// {
//     if (m_file.isOpen())
//         m_file.close();
// }

// void VoltageRecorder::setLatestSamples(const QVector<double> &s)
// {
//     if (s.size() == 4)
//     {
//         m_lastSamples = s;
//         m_hasSamples = true;
//     }
// }

// void VoltageRecorder::recordFrame(double timestampMs)
// {
//     if (!m_hasSamples) return;

//     m_ts << timestampMs << ","
//          << m_lastSamples[0] << ","
//          << m_lastSamples[1] << ","
//          << m_lastSamples[2] << ","
//          << m_lastSamples[3] << "\n";
// }

#include "voltagerecorder.h"
#include <QFile>
#include <QTextStream>

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
    out << "time_s, ai0, ai1, ai2, ai3\n";

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
