#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


// #include <NIDAQmx.h>
// #include <iostream>

// int main() {

//     char devNames[2048] = {0};
//     int32 err = DAQmxGetSysDevNames(devNames, sizeof(devNames));

//     if (err < 0) {
//         std::cout << "Error = " << err << std::endl;
//     } else {
//         std::cout << "Detected devices = " << devNames << std::endl;
//     }

//     return 0;
// }
