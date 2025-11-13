#include "syncmanager.h"

// 获取单例实例
SyncManager& SyncManager::instance() {
    static SyncManager inst;
    return inst;
}

// 构造函数
SyncManager::SyncManager() {
    startTime = std::chrono::steady_clock::now();
}

// 析构函数
SyncManager::~SyncManager() {}

// 标记开始时间
void SyncManager::markStart() {
    startTime = std::chrono::steady_clock::now();
}

// 获取相对时间（单位：毫秒）
double SyncManager::nowMs() {
    auto t = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::milli>(t - startTime).count();
}
