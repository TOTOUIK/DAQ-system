#include "DataRecorder.h"
#include <QDir>
#include <QDebug>

DataRecorder& DataRecorder::instance() {
    static DataRecorder inst;
    return inst;
}

DataRecorder::DataRecorder() : isRecording(false) {}

DataRecorder::~DataRecorder() {
    stopRecording();
}

void DataRecorder::startRecording() {
    std::lock_guard<std::mutex> lock(mtx);

    if (isRecording) return;

    QString baseDir = "data";
    QDir().mkpath(baseDir);

    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    QString forcePath = QString("%1/force_%2.csv").arg(baseDir).arg(timestamp);
    QString vibrationPath = QString("%1/vibration_%2.csv").arg(baseDir).arg(timestamp);

    forceFile.setFileName(forcePath);
    vibrationFile.setFileName(vibrationPath);

    if (!forceFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "无法打开三分力文件:" << forcePath;
        return;
    }
    if (!vibrationFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "无法打开振动文件:" << vibrationPath;
        return;
    }

    forceStream.setDevice(&forceFile);
    vibrationStream.setDevice(&vibrationFile);

    // 写入 CSV 表头
    forceStream << "timestamp_ms,Fx,Fy,Fz\n";
    vibrationStream << "timestamp_ms,value\n";

    isRecording = true;
    qDebug() << "数据记录启动，文件已创建：" << forcePath << vibrationPath;
}

void DataRecorder::stopRecording() {
    std::lock_guard<std::mutex> lock(mtx);
    if (!isRecording) return;

    forceStream.flush();
    vibrationStream.flush();
    forceFile.close();
    vibrationFile.close();
    isRecording = false;

    qDebug() << "数据记录已停止，文件已保存。";
}

void DataRecorder::recordForce(double timestamp, double fx, double fy, double fz) {
    std::lock_guard<std::mutex> lock(mtx);
    if (!isRecording) return;
    forceStream << QString("%1,%2,%3,%4\n")
                       .arg(timestamp, 0, 'f', 3)
                       .arg(fx, 0, 'f', 3)
                       .arg(fy, 0, 'f', 3)
                       .arg(fz, 0, 'f', 3);
}

void DataRecorder::recordVibration(double timestamp, double vibration) {
    std::lock_guard<std::mutex> lock(mtx);
    if (!isRecording) return;
    vibrationStream << QString("%1,%2\n")
                           .arg(timestamp, 0, 'f', 3)
                           .arg(vibration, 0, 'f', 3);
}
