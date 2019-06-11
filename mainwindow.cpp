#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_serialPortSearchButton_clicked() {
    qDebug() << "MainWindow::on_serialPortSearchButton_clicked " ;

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "   Name=" << info.portName()
                 << " Description=" << info.description()
                 << " Manufacturer=" << info.manufacturer();
        // Example use QSerialPort
//        QSerialPort serial;
//        serial.setPort(info);
//        if (serial.open(QIODevice::ReadWrite))
//            serial.close();
    }
}
