#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "syncmanager.h"
#include <QDebug>
#include <QMessageBox>
#include <QGraphicsScene>
#include <opencv2/opencv.hpp>
#include "cameracontroller.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , forceModule(new ForceModule(this))
    , vibrationModule(new VibrationModule(this))
{
    ui->setupUi(this);

    // 信号连接
    connect(forceModule, &ForceModule::newForceData, this, [](double t, double fx, double fy, double fz){
        Q_UNUSED(t); Q_UNUSED(fx); Q_UNUSED(fy); Q_UNUSED(fz);
    });

    connect(vibrationModule, &VibrationModule::newVibrationData, this, [](double t, double value){
        Q_UNUSED(t); Q_UNUSED(value);
    });

    // 相机错误提示
    connect(&camera, &CameraController::cameraError, this, [this](const QString& msg){
        QMessageBox::critical(this, "相机错误", msg);
    });

    // 初始化相机
    if (!camera.initCamera()) {
        QMessageBox::warning(this, "提示", "相机初始化失败！");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*==============================
 * 启动 / 停止采集
 *==============================*/
void MainWindow::on_startButton_clicked()
{
    SyncManager::instance().markStart();
    forceModule->start();
    vibrationModule->start();
    qDebug() << "采集启动！";
}

void MainWindow::on_stopButton_clicked()
{
    if (forceModule) forceModule->stop();
    if (vibrationModule) vibrationModule->stop();
    qDebug() << "采集停止。";
}

/*==============================
 * 图像显示辅助函数
 *==============================*/
static void showImageOnGraphicsView(QGraphicsView* view, const QImage& img)
{
    auto* scene = new QGraphicsScene(view);
    scene->addPixmap(QPixmap::fromImage(img));
    view->setScene(scene);
    view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

/*==============================
 * 图像预览
 *==============================*/
void MainWindow::on_btnPreview_clicked()
{
    QImage img;
    if (camera.capturePreview(img, ui->spinExposure->value())) {
        showImageOnGraphicsView(ui->graphicsCamera, img);
        qDebug() << "预览图显示成功";
    } else {
        QMessageBox::warning(this, "提示", "图像预览失败！");
    }
}

/*==============================
 * 深度图测量
 *==============================*/
void MainWindow::on_btnMeasure3D_clicked()
{
    QImage depthImg;
    gc3d::GC3DMetaData meta;

    bool ok = camera.captureDepth(
        depthImg, meta,
        ui->spinExposure->value(),
        ui->spinThresholdMin->value(),
        ui->spinThresholdMax->value(),
        ui->spinSmoothMin->value(),
        ui->spinSmoothMax->value(),
        ui->spinDenoise1->value(),  // 降噪参数1
        ui->spinDenoise2->value()   // 降噪参数2
        );

    if (ok) {
        showImageOnGraphicsView(ui->graphicsCamera, depthImg);
        qDebug() << "深度图显示成功";
    } else {
        QMessageBox::warning(this, "提示", "深度图采集失败！");
    }
}
