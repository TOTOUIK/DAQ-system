#ifndef FORCEMODULE_H
#define FORCEMODULE_H

#include <QObject>
#include <QThread>
#include <atomic>

class ForceModule : public QObject
{
    Q_OBJECT
public:
    explicit ForceModule(QObject *parent = nullptr);
    ~ForceModule();

public slots:
    void start();   // 开始采集
    void stop();    // 停止采集

signals:
    void newForceData(double timestamp, double fx, double fy, double fz);

private:
    void runLoop();  // 模拟采集线程函数
    QThread workerThread;
    std::atomic<bool> running;
};

#endif // FORCEMODULE_H

