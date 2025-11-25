#pragma once
#include <QString>
#include <QDateTime>

// class DataRecorderBase
// {
// public:
//     virtual ~DataRecorderBase() = default;

//     // 初始化保存目录（在 start 前调用）
//     virtual bool prepare(const QString& rootDir) = 0;

//     // 开始写入（创建文件、写入头）
//     virtual bool start() = 0;

//     // 停止写入（关闭文件）
//     virtual void stop() = 0;

//     // 每帧写入一次
//     // timestampMs = SyncManager::instance().nowMs()
//     virtual void recordFrame(double timestampMs) = 0;

// protected:
//     QString m_saveDir;     // SaveData/xxx/
//     QString m_filename;    // 文件名（子类决定）
// };
#pragma once
#include <QString>

class DataRecorderBase
{
public:
    virtual ~DataRecorderBase() = default;

    // 保存文件（返回 true / false）
    virtual bool saveToFile(const QString &filePath) = 0;

    // 清空缓存
    virtual void clear() = 0;

    // 是否有数据可保存
    virtual bool hasData() const = 0;
};
