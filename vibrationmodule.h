#ifndef VIBRATIONMODULE_H
#define VIBRATIONMODULE_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QRandomGenerator>
#include <QtMath>
#include "syncmanager.h"
#include "systemconfig.h"

class VibrationModule : public QObject
{
    Q_OBJECT
public:
    explicit VibrationModule(QObject *parent = nullptr);
    ~VibrationModule();

    void start();
    void stop();

signals:
    void newVibrationData(double timestamp, double vibration);

private:
    QThread workerThread;
    bool running;
    double timeSec = 0.0;   // ✅ 关键修复点：作为成员变量
};

#endif // VIBRATIONMODULE_H
