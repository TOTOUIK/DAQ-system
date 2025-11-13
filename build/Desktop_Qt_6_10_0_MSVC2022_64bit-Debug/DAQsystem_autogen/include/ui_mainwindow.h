/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QGraphicsView *graphicsVibration;
    QGraphicsView *graphicsCamera;
    QGraphicsView *graphicsForce;
    QWidget *controlPanel;
    QPushButton *stopButton;
    QPushButton *startButton;
    QGroupBox *groupBox;
    QPushButton *btnPreview;
    QPushButton *btnMeasure3D;
    QPushButton *btnSaveCloud;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpinBox *spinExposure;
    QLabel *label_2;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QSpinBox *spinHeightMin;
    QSpinBox *spinHeightMax;
    QLabel *label_4;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_5;
    QSpinBox *spinThresholdMin;
    QSpinBox *spinThresholdMax;
    QLabel *label_6;
    QWidget *horizontalLayoutWidget_5;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_10;
    QSpinBox *spinSmoothMin;
    QSpinBox *spinSmoothMax;
    QLabel *label_11;
    QWidget *horizontalLayoutWidget_6;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_12;
    QDoubleSpinBox *spinDenoise1;
    QLabel *label_13;
    QWidget *horizontalLayoutWidget_4;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_9;
    QSpinBox *spinDenoise2;
    QLabel *label_8;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1726, 895);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayoutWidget = new QWidget(centralwidget);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(40, 160, 1661, 681));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setSizeConstraint(QLayout::SizeConstraint::SetDefaultConstraint);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        graphicsVibration = new QGraphicsView(gridLayoutWidget);
        graphicsVibration->setObjectName("graphicsVibration");

        gridLayout->addWidget(graphicsVibration, 1, 0, 1, 1);

        graphicsCamera = new QGraphicsView(gridLayoutWidget);
        graphicsCamera->setObjectName("graphicsCamera");

        gridLayout->addWidget(graphicsCamera, 0, 2, 2, 1);

        graphicsForce = new QGraphicsView(gridLayoutWidget);
        graphicsForce->setObjectName("graphicsForce");

        gridLayout->addWidget(graphicsForce, 0, 0, 1, 1);

        controlPanel = new QWidget(centralwidget);
        controlPanel->setObjectName("controlPanel");
        controlPanel->setGeometry(QRect(40, 10, 581, 91));
        stopButton = new QPushButton(controlPanel);
        stopButton->setObjectName("stopButton");
        stopButton->setGeometry(QRect(120, 10, 93, 28));
        startButton = new QPushButton(controlPanel);
        startButton->setObjectName("startButton");
        startButton->setGeometry(QRect(10, 10, 93, 28));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(640, 10, 1071, 141));
        btnPreview = new QPushButton(groupBox);
        btnPreview->setObjectName("btnPreview");
        btnPreview->setGeometry(QRect(870, 30, 91, 41));
        btnMeasure3D = new QPushButton(groupBox);
        btnMeasure3D->setObjectName("btnMeasure3D");
        btnMeasure3D->setGeometry(QRect(970, 30, 91, 41));
        btnSaveCloud = new QPushButton(groupBox);
        btnSaveCloud->setObjectName("btnSaveCloud");
        btnSaveCloud->setGeometry(QRect(970, 80, 91, 41));
        horizontalLayoutWidget = new QWidget(groupBox);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(20, 30, 191, 41));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        spinExposure = new QSpinBox(horizontalLayoutWidget);
        spinExposure->setObjectName("spinExposure");
        spinExposure->setMinimum(1);
        spinExposure->setMaximum(30);
        spinExposure->setValue(5);

        horizontalLayout->addWidget(spinExposure);

        label_2 = new QLabel(horizontalLayoutWidget);
        label_2->setObjectName("label_2");

        horizontalLayout->addWidget(label_2);

        horizontalLayoutWidget_2 = new QWidget(groupBox);
        horizontalLayoutWidget_2->setObjectName("horizontalLayoutWidget_2");
        horizontalLayoutWidget_2->setGeometry(QRect(20, 80, 251, 41));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(horizontalLayoutWidget_2);
        label_3->setObjectName("label_3");

        horizontalLayout_3->addWidget(label_3);

        spinHeightMin = new QSpinBox(horizontalLayoutWidget_2);
        spinHeightMin->setObjectName("spinHeightMin");
        spinHeightMin->setMinimum(-50);
        spinHeightMin->setMaximum(50);
        spinHeightMin->setValue(-10);

        horizontalLayout_3->addWidget(spinHeightMin);

        spinHeightMax = new QSpinBox(horizontalLayoutWidget_2);
        spinHeightMax->setObjectName("spinHeightMax");
        spinHeightMax->setMinimum(-50);
        spinHeightMax->setMaximum(50);
        spinHeightMax->setValue(10);

        horizontalLayout_3->addWidget(spinHeightMax);

        label_4 = new QLabel(horizontalLayoutWidget_2);
        label_4->setObjectName("label_4");

        horizontalLayout_3->addWidget(label_4);

        horizontalLayoutWidget_3 = new QWidget(groupBox);
        horizontalLayoutWidget_3->setObjectName("horizontalLayoutWidget_3");
        horizontalLayoutWidget_3->setGeometry(QRect(290, 30, 261, 41));
        horizontalLayout_4 = new QHBoxLayout(horizontalLayoutWidget_3);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(horizontalLayoutWidget_3);
        label_5->setObjectName("label_5");

        horizontalLayout_4->addWidget(label_5);

        spinThresholdMin = new QSpinBox(horizontalLayoutWidget_3);
        spinThresholdMin->setObjectName("spinThresholdMin");
        spinThresholdMin->setMaximum(255);

        horizontalLayout_4->addWidget(spinThresholdMin);

        spinThresholdMax = new QSpinBox(horizontalLayoutWidget_3);
        spinThresholdMax->setObjectName("spinThresholdMax");
        spinThresholdMax->setMaximum(255);
        spinThresholdMax->setValue(255);

        horizontalLayout_4->addWidget(spinThresholdMax);

        label_6 = new QLabel(horizontalLayoutWidget_3);
        label_6->setObjectName("label_6");

        horizontalLayout_4->addWidget(label_6);

        horizontalLayoutWidget_5 = new QWidget(groupBox);
        horizontalLayoutWidget_5->setObjectName("horizontalLayoutWidget_5");
        horizontalLayoutWidget_5->setGeometry(QRect(290, 80, 261, 41));
        horizontalLayout_6 = new QHBoxLayout(horizontalLayoutWidget_5);
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        label_10 = new QLabel(horizontalLayoutWidget_5);
        label_10->setObjectName("label_10");

        horizontalLayout_6->addWidget(label_10);

        spinSmoothMin = new QSpinBox(horizontalLayoutWidget_5);
        spinSmoothMin->setObjectName("spinSmoothMin");
        spinSmoothMin->setMaximum(20);

        horizontalLayout_6->addWidget(spinSmoothMin);

        spinSmoothMax = new QSpinBox(horizontalLayoutWidget_5);
        spinSmoothMax->setObjectName("spinSmoothMax");
        spinSmoothMax->setMaximum(20);
        spinSmoothMax->setValue(20);

        horizontalLayout_6->addWidget(spinSmoothMax);

        label_11 = new QLabel(horizontalLayoutWidget_5);
        label_11->setObjectName("label_11");

        horizontalLayout_6->addWidget(label_11);

        horizontalLayoutWidget_6 = new QWidget(groupBox);
        horizontalLayoutWidget_6->setObjectName("horizontalLayoutWidget_6");
        horizontalLayoutWidget_6->setGeometry(QRect(630, 30, 181, 41));
        horizontalLayout_7 = new QHBoxLayout(horizontalLayoutWidget_6);
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        label_12 = new QLabel(horizontalLayoutWidget_6);
        label_12->setObjectName("label_12");

        horizontalLayout_7->addWidget(label_12);

        spinDenoise1 = new QDoubleSpinBox(horizontalLayoutWidget_6);
        spinDenoise1->setObjectName("spinDenoise1");
        spinDenoise1->setMinimum(0.100000000000000);
        spinDenoise1->setMaximum(50.000000000000000);

        horizontalLayout_7->addWidget(spinDenoise1);

        label_13 = new QLabel(horizontalLayoutWidget_6);
        label_13->setObjectName("label_13");

        horizontalLayout_7->addWidget(label_13);

        horizontalLayoutWidget_4 = new QWidget(groupBox);
        horizontalLayoutWidget_4->setObjectName("horizontalLayoutWidget_4");
        horizontalLayoutWidget_4->setGeometry(QRect(630, 80, 181, 41));
        horizontalLayout_5 = new QHBoxLayout(horizontalLayoutWidget_4);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_9 = new QLabel(horizontalLayoutWidget_4);
        label_9->setObjectName("label_9");

        horizontalLayout_5->addWidget(label_9);

        spinDenoise2 = new QSpinBox(horizontalLayoutWidget_4);
        spinDenoise2->setObjectName("spinDenoise2");
        spinDenoise2->setMinimum(10);
        spinDenoise2->setMaximum(50);

        horizontalLayout_5->addWidget(spinDenoise2);

        label_8 = new QLabel(horizontalLayoutWidget_4);
        label_8->setObjectName("label_8");

        horizontalLayout_5->addWidget(label_8);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1726, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        stopButton->setText(QCoreApplication::translate("MainWindow", "\347\273\223\346\235\237", nullptr));
        startButton->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\347\233\270\346\234\272\350\256\276\347\275\256", nullptr));
        btnPreview->setText(QCoreApplication::translate("MainWindow", "\345\233\276\345\203\217\351\242\204\350\247\210", nullptr));
        btnMeasure3D->setText(QCoreApplication::translate("MainWindow", "\346\267\261\345\272\246\345\233\276", nullptr));
        btnSaveCloud->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230\346\225\260\346\215\256", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\346\233\235\345\205\211\346\227\266\351\227\264", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "1 ~ 30 ms", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\351\253\230\345\272\246\350\214\203\345\233\264", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "-50-50mm", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\351\230\210\345\200\274\350\256\276\347\275\256", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "0-255", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "\347\202\271\344\272\221\345\271\263\346\273\221", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "0-20", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "\351\231\215\345\231\252\346\214\207\346\225\260", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "0.1-5", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "\351\231\215\345\231\252\346\214\207\346\225\2602", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "10-50", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
