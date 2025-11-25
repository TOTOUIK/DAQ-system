#pragma once
#include "datarecorderbase.h"
#include <QString>
#include <QVector>
#include <QMutex>

    class CameraController;

class CameraRecorder : public DataRecorderBase
{
public:
    explicit CameraRecorder(CameraController* cam);
    ~CameraRecorder() override = default;

    // 可选：创建保存目录并初始化（不是基类接口，但对使用方便）
    bool prepare(const QString &rootDir);

    // 记录一帧（timestamp 单位：秒）
    void append(double timestamp);

    // DataRecorderBase 接口
    bool saveToFile(const QString &filePath) override;
    void clear() override;
    bool hasData() const override;

private:
    struct FrameEntry {
        QString filename;
        double timestamp;
    };

    CameraController* m_cam = nullptr;
    QString m_saveDir;
    QVector<FrameEntry> frames;
    mutable QMutex mutex;
};
