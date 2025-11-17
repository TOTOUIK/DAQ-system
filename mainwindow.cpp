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
    // 深度视图/鼠标坐标工具
    depthView = new DepthView(this);
    depthView->attachToView(ui->graphicsCamera);
    depthView->setLabelXYZ(ui->labelXYZ);
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
// static void showImageOnGraphicsView(QGraphicsView* view, const QImage& img)
// {
//     qDebug() << "进入图像展示";
//     auto* scene = new QGraphicsScene(view);
//     scene->addPixmap(QPixmap::fromImage(img));
//     view->setScene(scene);
//     view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
// }
void MainWindow::showImageOnGraphicsView(QGraphicsView *view, const QImage &img)
{
    if (!view) return;
    if (img.isNull()) return;

    // new scene each time（简单做法）——如果频繁更新可复用 scene / pixmapItem 提高效率
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addPixmap(QPixmap::fromImage(img));
    view->setScene(scene);
    view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    view->setSceneRect(scene->itemsBoundingRect());
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
// void MainWindow::on_btnPreview_clicked()
// {
//     // 先把参数应用到相机（预览主要需要曝光）
//     int exposure = ui->spinExposure->value();
//     // 你可以把 applyParameters 放到 CameraController 中，下面简单调用 setExposure
//     cameraController->setExposure(exposure);

//     QImage preview;
//     if (cameraController->capturePreview(preview)) {
//         // 把 meta 也设置给 depthView（预览不改变 meta，但设置为最后 meta 若有）
//         depthView->setMetaData(cameraController->getLastMeta()); // 需要 CameraController 提供 getLastMeta()
//         showImageOnGraphicsView(ui->graphicsCamera, preview);
//     } else {
//         QMessageBox::warning(this, "提示", "图像预览失败！");
//     }
// }
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
    QImage depthImg;
    gc3d::GC3DMetaData meta;
    if (cameraController->captureDepth(depthImg, meta)) {
        // 重要：先把 meta 存到 CameraController（由 CameraController 管理生命周期）
        cameraController->setLastMeta(meta);
        // 再把 CameraController 管理的 meta 指针传给 depthView（避免传递局部地址）
        depthView->setMetaData(cameraController->getLastMeta());
        showImageOnGraphicsView(ui->graphicsCamera, depthImg);
    } else {
        QMessageBox::warning(this, "错误", "深度扫描失败！");
    }
}

void MainWindow::on_btnSaveCloud_clicked()
{
    // 把保存工作委托给 CameraController（CameraController 内部使用 lastMeta）
    // 生成保存目录与文件名
    QDir rootDir(QCoreApplication::applicationDirPath());
    // 你想放项目根目录 SaveData：向上一级再建 SaveData（根据你的工程目录结构调整）
    QString saveDirPath = rootDir.filePath("/SaveData");
    QDir saveDir(saveDirPath);
    if (!saveDir.exists()) {
        saveDir.mkpath(".");
    }

    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    QString fname = saveDir.filePath(QString("pointcloud_%1.npy").arg(timestamp));

    bool ok = cameraController->saveLastPointCloudNpy(fname.toStdString());
    if (ok) {
        QMessageBox::information(this, "保存成功", QString("已保存点云：%1").arg(fname));
    } else {
        QMessageBox::warning(this, "保存失败", "点云保存失败或没有可用数据。");
    }
}

