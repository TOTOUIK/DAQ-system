#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QDir>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QtCharts>
#include "cameracontroller.h"
#include "graphicsviewzoomer.h"
#include "nidaqcontroller.h"
#include "DepthView.h"
#include "camerarecorder.h"
#include "voltagerecorder.h"
QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);   // ✅ 保留一份
    ~MainWindow();

private slots:
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void on_btnPreview_clicked();     // ✅ 预览图
    void on_btnDepth_clicked();   // ✅ 深度图
    void on_btnSaveCloud_clicked();
    void showImageOnGraphicsView(QGraphicsView *view, const QImage &img);
    void setupCharts();
    void onNIStartClicked();
    void onNIStopClicked();
    void updateCharts();
    void onSamples1(const QVector<double>& s, double t);
    void onSamples2(const QVector<double>& s, double t);
    void onNISaveButtonClicked();
private:
    Ui::MainWindow *ui;
    CameraController *cameraController = nullptr;
    GraphicsViewZoomer *cameraZoomer = nullptr;
    QGraphicsScene *cameraScene = nullptr;
    QString getTimestamp();
    DepthView* depthView;

    NIDaqController *nidaqController = nullptr;

    // 两个图表（每个图 4 条曲线）
    QChart *chart1 = nullptr;
    QChart *chart2 = nullptr;

    QLineSeries *series1[4];
    QLineSeries *series2[4];

    QValueAxis *axisX1;
    QValueAxis *axisY1;
    QValueAxis *axisX2;
    QValueAxis *axisY2;

    QTimer updateTimer;                // 控制刷新频率（避免每点都刷新）
    QVector<double> lastSamples1;      // 插槽1 缓存
    QVector<double> lastSamples2;      // 插槽2 缓存
    double lastTimestamp = 0;
    QVector<double> last1, last2;
    double ts1 = 0, ts2 = 0;

    CameraRecorder*  cameraRecorder;
    VoltageRecorder* voltageRecorder;
};

#endif // MAINWINDOW_H
