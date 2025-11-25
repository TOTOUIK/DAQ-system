#pragma once
#include <vector>
#include <cstdint>

// 一帧点云数据（用于同步模块保存）
struct PointCloudFrame
{
    double timestampMs = 0.0;   // 使用 SyncManager::nowMs()

    // 点云数据（已过滤 maskflag）
    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> z;

    // 点的数量 (x.size() == y.size() == z.size())
    size_t count() const { return x.size(); }
};
