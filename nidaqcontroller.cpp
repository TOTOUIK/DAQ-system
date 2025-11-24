#include "nidaqcontroller.h"
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

    return true;
}



bool NIDaqController::configureChannels(const QStringList &, bool)
{
    return false;
}

void NIDaqController::setSampleRate(double rate)
{
    m_sampleRateHz = rate;
}

bool NIDaqController::start()
{
    return false;
}

void NIDaqController::stop()
{
}

bool NIDaqController::isRunning() const
{
    return m_running.load();
}

QVector<double> NIDaqController::filterSamples(const QVector<double> &samples)
{
    return samples;
}

QStringList NIDaqController::expandPhysicalChannels(const QStringList &chs)
{
    return chs;
}
