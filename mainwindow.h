#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QDir>
#include <QGraphicsView>       // ✅ 关键补充
#include <QGraphicsScene>
#include "forcemodule.h"
#include "vibrationmodule.h"
#include "cameracontroller.h"
#include "graphicsviewzoomer.h"
#include "DepthView.h"
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

private:
    Ui::MainWindow *ui;
    ForceModule *forceModule;
    VibrationModule *vibrationModule;
    CameraController *cameraController = nullptr;
    GraphicsViewZoomer *cameraZoomer = nullptr;
    QGraphicsScene *cameraScene = nullptr;
    void showMatOnGraphicsView(QGraphicsView *view, const cv::Mat &img); // ✅ 正确声明
    QString getTimestamp();
};

#endif // MAINWINDOW_H
