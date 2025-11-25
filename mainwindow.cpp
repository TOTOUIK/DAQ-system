#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "syncmanager.h"
#include <QDebug>
#include <QMessageBox>
#include <QGraphicsScene>
#include <opencv2/opencv.hpp>
#include "cameracontroller.h"
#include "nidaqcontroller.h"
#include "gc3d.h"
#include "gc3dAlgorithm.h"
#include "graphicsviewzoomer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
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
    // 创建 NIDaqController
    nidaqController = new NIDaqController(this);
    nidaqController->init();
    setupCharts();

    connect(ui->NIstartButton, &QPushButton::clicked, this, &MainWindow::onNIStartClicked);
    connect(ui->NIstopButton,  &QPushButton::clicked, this, &MainWindow::onNIStopClicked);

    connect(nidaqController, &NIDaqController::newSamples1,
            this, &MainWindow::onSamples1);
    connect(nidaqController, &NIDaqController::newSamples2,
            this, &MainWindow::onSamples2);
    connect(ui->NIsaveButton, &QPushButton::clicked,
            this, &MainWindow::onNISaveButtonClicked);

    // 刷新图表（30 FPS）
    updateTimer.setInterval(33);
    connect(&updateTimer, &QTimer::timeout, this, &MainWindow::updateCharts);
    // 保存数据
    voltageRecorder = new VoltageRecorder();
    // cameraRecorder  = new CameraRecorder(cameraController);


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

    qDebug() << "采集启动！";
}

void MainWindow::on_stopButton_clicked()
{

    qDebug() << "采集停止。";
}


QString MainWindow::getTimestamp()
{
    return QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
}
/*==============================
 * 图像显示辅助函数
 *==============================*/

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

void MainWindow::setupCharts()
{
    // --- 图1 ---
    chart1 = new QChart();
    axisX1 = new QValueAxis();
    axisY1 = new QValueAxis();

    axisX1->setRange(0, 5); // 显示5秒的窗口
    axisY1->setRange(-10, 10);

    chart1->addAxis(axisX1, Qt::AlignBottom);
    chart1->addAxis(axisY1, Qt::AlignLeft);

    for (int i = 0; i < 4; i++)
    {
        series1[i] = new QLineSeries();
        series1[i]->setName(QString("AI%1").arg(i));
        chart1->addSeries(series1[i]);
        series1[i]->attachAxis(axisX1);
        series1[i]->attachAxis(axisY1);
    }

    ui->chartVoltage1->setChart(chart1);
    ui->chartVoltage1->setRenderHint(QPainter::Antialiasing);

    // --- 图2 ---
    chart2 = new QChart();
    axisX2 = new QValueAxis();
    axisY2 = new QValueAxis();

    axisX2->setRange(0, 5);
    axisY2->setRange(-10, 10);

    chart2->addAxis(axisX2, Qt::AlignBottom);
    chart2->addAxis(axisY2, Qt::AlignLeft);

    for (int i = 0; i < 4; i++)
    {
        series2[i] = new QLineSeries();
        series2[i]->setName(QString("AI%1").arg(i));
        chart2->addSeries(series2[i]);
        series2[i]->attachAxis(axisX2);
        series2[i]->attachAxis(axisY2);
    }

    ui->chartVoltage2->setChart(chart2);
    ui->chartVoltage2->setRenderHint(QPainter::Antialiasing);
}




// ======================================================
//  开始采集
// ======================================================
// void MainWindow::onNIStartClicked()
// {

//     int slot1 = ui->graphicsmod1->currentIndex() + 1;  // 1~4
//     int slot2 = ui->graphicsmod2->currentIndex() + 1;  // 1~4

//     // 选择 slot1 对应的模块
//     nidaqController->configure(slot1);

//     // 设置采样率
//     double sr = ui->spinSampleRate->value();
//     nidaqController->setSampleRate(sr);

//     // 启动 DAQ
//     if (nidaqController->start())
//     {
//         updateTimer.start();
//         qDebug() << "DAQ started.";
//     }
//     else
//     {
//         qDebug() << "DAQ 启动失败";
//     }
// }
void MainWindow::onNIStartClicked()
{    // ---------- 清空旧图像 ----------
    voltageRecorder->clear();
    for (int i = 0; i < 4; i++) {
        series1[i]->clear();
        series2[i]->clear();
    }
    lastSamples1.clear();
    lastSamples2.clear();
    lastTimestamp = 0;
    int slot1 = ui->graphicsmod1->currentIndex();  // 0=无，1~4=插槽
    int slot2 = ui->graphicsmod2->currentIndex();

    double sr = ui->spinSampleRate->value();
    nidaqController->setSampleRate(sr);
    // 启动 DAQ
    if (nidaqController->startDual(slot1, slot2)) {
        updateTimer.start();
        qDebug() << "DAQ started.";
    } else {
        qDebug() << "DAQ 启动失败";
    }
}


// ======================================================
// 停止采集
// ======================================================
void MainWindow::onNIStopClicked()
{
    nidaqController->stopDual();
    updateTimer.stop();
    qDebug() << "DAQ stopped.";
}

// ======================================================
//  NI 回调的原始采样数据 (samples = 4 值)
// ======================================================

void MainWindow::onSamples1(const QVector<double>& s, double t)
{
    last1 = s;
    ts1 = t;
    voltageRecorder->append(s, t);
}

void MainWindow::onSamples2(const QVector<double>& s, double t)
{
    last2 = s;
    ts2 = t;
    voltageRecorder->append(s, t);
}

// ======================================================
//  图表刷新（30 FPS）
// ======================================================

void MainWindow::updateCharts()
{
    // 图1
    if (last1.size() == 4) {
        for (int i=0;i<4;i++)
            series1[i]->append(ts1, last1[i]);
        axisX1->setRange(ts1 - 5, ts1);
    }

    // 图2
    if (last2.size() == 4) {
        for (int i=0;i<4;i++)
            series2[i]->append(ts2, last2[i]);
        axisX2->setRange(ts2 - 5, ts2);
    }
}

//电压数据保存

void MainWindow::onNISaveButtonClicked()
{
    if (!voltageRecorder->hasData()) {
        QMessageBox::warning(this, "提示", "没有可以保存的电压数据！");
        return;
    }

    QString base = QCoreApplication::applicationDirPath() + "/SaveData";
    QDir dir(base);
    if (!dir.exists()) dir.mkpath(".");

    QString fname = base + "/voltage_" +
                    QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") +
                    ".csv";

    if (voltageRecorder->saveToFile(fname))
        QMessageBox::information(this, "保存成功", "电压数据已保存到：\n" + fname);
    else
        QMessageBox::warning(this, "保存失败", "保存过程出错！");
}


// void MainWindow::on_btnStartSave_clicked()
// {
//     SyncManager::instance().markStart();

//     QString rootDir = QCoreApplication::applicationDirPath() + "/SaveData";
//     QDir().mkpath(rootDir);

//     voltageRecorder->prepare(rootDir);
//     voltageRecorder->start();

//     cameraRecorder->prepare(rootDir);
//     cameraRecorder->start();

//     // 开一个定时器，每帧保存一次
//     connect(&recordTimer, &QTimer::timeout, this, [this](){
//         double t = SyncManager::instance().nowMs();
//         voltageRecorder->recordFrame(t);
//         cameraRecorder->recordFrame(t);
//     });

//     recordTimer.start(33); // ~30 FPS
// }
