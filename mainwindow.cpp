#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "syncmanager.h"
#include <QDebug>
#include <QMessageBox>
#include <QGraphicsScene>
#include <opencv2/opencv.hpp>
#include "cameracontroller.h"
#include "gc3d.h"
#include "gc3dAlgorithm.h"
#include "graphicsviewzoomer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    forceModule(new ForceModule(this)),
    vibrationModule(new VibrationModule(this)),
    cameraController(new CameraController(this))
{
    ui->setupUi(this);
    if (!cameraController->initCamera()) {
        QMessageBox::critical(this, "错误", "相机初始化失败！");
    } else {
        qDebug() << "相机初始化成功。";
    }
    // 图像显示设置
    cameraScene = new QGraphicsScene(this);
    ui->graphicsCamera->setScene(cameraScene);
    cameraZoomer = new GraphicsViewZoomer(ui->graphicsCamera, this);
    connect(ui->btnPreview, &QPushButton::clicked, this, &MainWindow::on_btnPreview_clicked);
    connect(ui->btnDepth, &QPushButton::clicked, this, &MainWindow::on_btnDepth_clicked);
    // 信号连接
    connect(forceModule, &ForceModule::newForceData, this, [](double t, double fx, double fy, double fz){
        Q_UNUSED(t); Q_UNUSED(fx); Q_UNUSED(fy); Q_UNUSED(fz);
    });

    connect(vibrationModule, &VibrationModule::newVibrationData, this, [](double t, double value){
        Q_UNUSED(t); Q_UNUSED(value);
    });

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


QString MainWindow::getTimestamp()
{
    return QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
}
/*==============================
 * 图像显示辅助函数
 *==============================*/
static void showImageOnGraphicsView(QGraphicsView* view, const QImage& img)
{
    qDebug() << "进入图像展示";
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
    // ====== 提取 UI 参数 ======
    int exposure   = ui->spinExposure->value();
    int smooth     = ui->spinSmooth->value();
    int thMin      = ui->spinThresholdMin->value();
    int thMax      = ui->spinThresholdMax->value();
    int heightMin  = ui->spinHeightMin->value();
    int heightMax  = ui->spinHeightMax->value();
    float dn1      = ui->spinDenoise1->value();
    float dn2      = ui->spinDenoise2->value();

    // ====== 设置参数 ======
    cameraController->applyParameters(
        exposure,
        smooth,
        thMin, thMax,
        heightMin, heightMax,
        dn1, dn2
        );

    // ====== 采集图像 ======
    QImage img;
    if (cameraController->capturePreview(img)) {
        showImageOnGraphicsView(ui->graphicsCamera, img);
    } else {
        QMessageBox::warning(this, "提示", "图像预览失败！");
    }
}

/*==============================
 * 深度图测量
 *==============================*/
void MainWindow::on_btnDepth_clicked()
{
    // ====== 提取 UI 参数 ======
    int exposure   = ui->spinExposure->value();
    int smooth     = ui->spinSmooth->value();
    int thMin      = ui->spinThresholdMin->value();
    int thMax      = ui->spinThresholdMax->value();
    int heightMin  = ui->spinHeightMin->value();
    int heightMax  = ui->spinHeightMax->value();
    float dn1      = ui->spinDenoise1->value();
    float dn2      = ui->spinDenoise2->value();

    // ====== 设置参数 ======
    cameraController->applyParameters(
        exposure,
        smooth,
        thMin, thMax,
        heightMin, heightMax,
        dn1, dn2
        );

    // ====== 扫描深度 ======
    QImage depthImg;
    gc3d::GC3DMetaData meta;
    if (cameraController->captureDepth(depthImg, meta)) {
        showImageOnGraphicsView(ui->graphicsCamera, depthImg);
    } else {
        QMessageBox::warning(this, "错误", "深度扫描失败！");
    }
}


// void MainWindow::on_btnSaveCloud_clicked()
// {
//     cameraController->savePointCloudXYZ("cloud.xyz", meta);

// }

