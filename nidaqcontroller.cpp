#include "nidaqcontroller.h"
#include "acquisitionworker.h"
#include <QDebug>
#include <QString>

// 必须放最前（内部依赖 Windows 头文件）
#include <NIDAQmx.h>

NIDaqController::NIDaqController(QObject *parent)
    : QObject(parent)
{
}

NIDaqController::~NIDaqController()
{
}

bool NIDaqController::init()
{
    char devNames[2048] = {0};

    // 获取所有 NI 设备名称，例如 "cDAQ1, cDAQ1Mod1, cDAQ1Mod2"
    int32 err = DAQmxGetSysDevNames(devNames, sizeof(devNames));
    if (err != 0 || strlen(devNames) == 0)
    {
        qDebug() << "[NI-DAQ] 未检测到任何设备";
        return false;
    }

    qDebug() << "[NI-DAQ] 检测到设备列表:" << devNames;

    // 拆分设备名
    QStringList devList = QString(devNames)
                              .replace(',', ' ')
                              .split(' ', Qt::SkipEmptyParts);

    // 保存最终结果
    QStringList output;

    // 先识别每一个设备的型号
    struct DevInfo {
        QString name;
        QString product;
    };
    QVector<DevInfo> devices;

    for (const QString &dev : devList)
    {
        char prod[512] = {0};
        err = DAQmxGetDevProductType(dev.toUtf8().data(), prod, sizeof(prod));

        QString prodStr = (err == 0 && strlen(prod) > 0) ?
                              QString(prod) : "Unknown";

        devices.push_back({dev, prodStr});
    }

    // 判断哪些是机箱（cDAQ 系列）
    for (const auto &info : devices)
    {
        if (info.product.contains("cDAQ", Qt::CaseInsensitive))
        {
            // 输出机箱信息
            output << QString("%1 : %2").arg(info.name, info.product);

            // 查找属于此机箱的模块（规则：模块名包含机箱名前缀）
            QString chassisPrefix = info.name;   // e.g. "cDAQ1"

            for (const auto &mod : devices)
            {
                if (mod.name.startsWith(chassisPrefix) && mod.name != info.name)
                {
                    output << QString("  %1 = %2").arg(mod.name, mod.product);
                }
            }
        }
    }

    // 如果没有检测到任何机箱
    if (output.isEmpty())
    {
        qDebug() << "[NI-DAQ] 未找到 cDAQ 机箱（可能只有独立设备）";
        return false;
    }

    // 最终输出
    for (const QString &line : output)
        qDebug().noquote() << line;

    // 保存机箱名
    for (const auto &info : devices)
    {
        if (info.product.contains("cDAQ", Qt::CaseInsensitive))
        {
            m_deviceChassis = info.name;

            // 保存模块列表
            QString prefix = info.name;
            for (const auto &mod : devices)
            {
                if (mod.name.startsWith(prefix) && mod.name != info.name)
                    m_availableModules << mod.name;
            }
        }
    }

    return true;
}

bool NIDaqController::configure(int slot1, int slot2)
{
    m_selectedModule1.clear();
    m_selectedModule2.clear();

    if (slot1 >= 1 && slot1 <= 4)
    {
        QString mod1 = QString("%1Mod%2").arg(m_deviceChassis).arg(slot1);
        if (m_availableModules.contains(mod1))
            m_selectedModule1 = mod1;
    }

    if (slot2 >= 1 && slot2 <= 4)
    {
        QString mod2 = QString("%1Mod%2").arg(m_deviceChassis).arg(slot2);
        if (m_availableModules.contains(mod2))
            m_selectedModule2 = mod2;
    }

    if (m_selectedModule1.isEmpty() && m_selectedModule2.isEmpty())
        return false;

    qDebug() << "[NI] 选择模块:" << m_selectedModule1 << m_selectedModule2;

    return true;
}

void NIDaqController::setSampleRate(double r)
{
    m_sampleRate = r;
}

// bool NIDaqController::start()
// {
//     if (m_selectedModule.isEmpty()) return false;

//     stop();

//     m_thread = new QThread();
//     m_worker = new AcquisitionWorker(m_selectedModule, m_sampleRate);

//     m_worker->moveToThread(m_thread);

//     connect(m_thread, &QThread::started, m_worker, &AcquisitionWorker::process);
//     connect(m_worker, &AcquisitionWorker::newSamples, this, &NIDaqController::newSamples);
//     connect(m_worker, &AcquisitionWorker::finished, m_thread, &QThread::quit);

//     m_thread->start();
//     m_running = true;
//     return true;
// }

// void NIDaqController::stop()
// {
//     if (!m_thread) return;

//     m_worker->stop();
//     m_thread->quit();
//     m_thread->wait();

//     delete m_worker;
//     delete m_thread;

//     m_worker = nullptr;
//     m_thread = nullptr;

//     m_running = false;
// }

bool NIDaqController::startDual(int slot1, int slot2)
{
    stopDual();  // 清理旧线程

    // --------- slot1 ----------
    QString mod1 = QString("%1Mod%2").arg(m_deviceChassis).arg(slot1);
    if (!m_availableModules.contains(mod1)) {
        qDebug() << "[NI] slot1 无模块";
        mod1.clear();
    }

    // --------- slot2 ----------
    QString mod2 = QString("%1Mod%2").arg(m_deviceChassis).arg(slot2);
    if (!m_availableModules.contains(mod2)) {
        qDebug() << "[NI] slot2 无模块";
        mod2.clear();
    }

    // 若两个都为空，则不启动
    if (mod1.isEmpty() && mod2.isEmpty()) {
        qDebug() << "[NI] 无可采集模块，停止";
        return false;
    }

    // ---------- 创建 worker1 ----------
    if (!mod1.isEmpty()) {
        qDebug() << "slot1启动";
        thread1 = new QThread();
        worker1 = new AcquisitionWorker(mod1, m_sampleRate);

        worker1->moveToThread(thread1);

        connect(thread1, &QThread::started, worker1, &AcquisitionWorker::process);
        connect(worker1, &AcquisitionWorker::newSamples, this, &NIDaqController::newSamples1);
        connect(worker1, &AcquisitionWorker::finished, thread1, &QThread::quit);

        thread1->start();
    }

    // ---------- 创建 worker2 ----------
    if (!mod2.isEmpty()) {
        thread2 = new QThread();
        worker2 = new AcquisitionWorker(mod2, m_sampleRate);

        worker2->moveToThread(thread2);

        connect(thread2, &QThread::started, worker2, &AcquisitionWorker::process);
        connect(worker2, &AcquisitionWorker::newSamples, this, &NIDaqController::newSamples2);
        connect(worker2, &AcquisitionWorker::finished, thread2, &QThread::quit);

        thread2->start();
    }

    return true;
}

void NIDaqController::stopDual()
{
    if (worker1) worker1->stop();
    if (worker2) worker2->stop();

    if (thread1) { thread1->quit(); thread1->wait(); }
    if (thread2) { thread2->quit(); thread2->wait(); }

    delete worker1; worker1 = nullptr;
    delete thread1; thread1 = nullptr;

    delete worker2; worker2 = nullptr;
    delete thread2; thread2 = nullptr;
}

bool NIDaqController::isRunning() const
{
    return m_running;
}
