#ifndef SYNCMANAGER_H
#define SYNCMANAGER_H

#include <chrono>

// 全局时间同步管理类（单例）
class SyncManager
{
public:
    // 获取唯一实例（全局唯一）
    static SyncManager& instance();

    // 标记采集开始时间（点击“开始采集”时调用一次）
    void markStart();

    // 获取相对于开始时间的当前时间（单位：毫秒）
    double nowMs();

private:
    // 私有构造函数（单例模式）
    SyncManager();
    ~SyncManager();

    // 起始时间点
    std::chrono::steady_clock::time_point startTime;
};

#endif // SYNCMANAGER_H
