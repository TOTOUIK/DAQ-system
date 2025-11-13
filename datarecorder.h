#ifndef DATARECORDER_H
#define DATARECORDER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <mutex>

// 通用数据记录模块
class DataRecorder
{
public:
    static DataRecorder& instance();

    // 开始新的数据记录（生成新文件）
    void startRecording();

    // 停止记录，关闭文件
    void stopRecording();

    // 记录三分力数据
    void recordForce(double timestamp, double fx, double fy, double fz);

    // 记录振动数据
    void recordVibration(double timestamp, double vibration);

private:
    DataRecorder();
    ~DataRecorder();

    QFile forceFile;
    QFile vibrationFile;
    QTextStream forceStream;
    QTextStream vibrationStream;

    bool isRecording;
    std::mutex mtx;
};

#endif // DATARECORDER_H
