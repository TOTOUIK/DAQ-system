#include "AcquisitionWorker.h"
#include <QDebug>

AcquisitionWorker::AcquisitionWorker(const QString &physicalDevice, double sampleRate)
    : m_dev(physicalDevice), m_rate(sampleRate)
{
}

AcquisitionWorker::~AcquisitionWorker()
{
    if (m_task)
        DAQmxClearTask(m_task);
}

void AcquisitionWorker::stop()
{
    m_running = false;
}

void AcquisitionWorker::process()
{
    int32 err = 0;

    // 创建任务
    err = DAQmxCreateTask("", &m_task);
    if (err) return;

    // 4 通道：ai0–ai3
    QString ch = m_dev + "/ai0:3";

    err = DAQmxCreateAIVoltageChan(
        m_task,
        ch.toUtf8().data(),
        "",
        DAQmx_Val_Cfg_Default,
        -10.0,
        10.0,
        DAQmx_Val_Volts,
        nullptr
        );
    if (err) return;

    DAQmxCfgSampClkTiming(
        m_task,
        "",
        m_rate,
        DAQmx_Val_Rising,
        DAQmx_Val_ContSamps,
        100
        );

    err = DAQmxStartTask(m_task);
    if (err) return;

    QVector<double> buffer(4);
    QElapsedTimer timer;
    timer.start();

    while (m_running)
    {
        int32 samps = 0;
        float64 data[4];

        err = DAQmxReadAnalogF64(
            m_task,
            1,
            0.5,
            DAQmx_Val_GroupByChannel,
            data,
            4,
            &samps,
            nullptr
            );

        if (err == 0 && samps == 1)
        {
            buffer[0] = data[0];
            buffer[1] = data[1];
            buffer[2] = data[2];
            buffer[3] = data[3];

            emit newSamples(buffer, timer.elapsed() * 0.001);
        }
    }

    DAQmxStopTask(m_task);
    emit finished();
}
