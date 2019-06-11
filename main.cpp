#include "mainwindow.h"

#include <QApplication>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "main::main";

//    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
//        qDebug() << "   Name=" << info.portName()
//                 << " Description=" << info.description()
//                 << " Manufacturer=" << info.manufacturer();
//        // Example use QSerialPort
//        QSerialPort serial;
//        serial.setPort(info);
//        if (serial.open(QIODevice::ReadWrite))
//            serial.close();
//    }

    MainWindow w;
    w.show();



    return a.exec();
}
