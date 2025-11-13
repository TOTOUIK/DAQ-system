#ifndef SYSTEMCONFIG_H
#define SYSTEMCONFIG_H

// 系统全局参数（可扩展）
struct SystemConfig {
    static inline constexpr int SAMPLE_INTERVAL_MS = 100;  // 采样周期(ms)
    static inline constexpr double SAMPLE_FREQ_HZ = 1000.0 / SAMPLE_INTERVAL_MS;  // 采样频率(Hz)
};

#endif // SYSTEMCONFIG_H
