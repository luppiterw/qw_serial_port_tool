#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QDebug>
#include <QDateTime>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    if(mCurrentSerialPort.isOpen())
        mCurrentSerialPort.close();
    delete ui;
}

void MainWindow::on_serialPortSearchButton_clicked() {
    qDebug() << "MainWindow::on_serialPortSearchButton_clicked " ;

    mCachedSeralPortInfoList.clear();
    ui->serialPortListWidget->clear();

//    mCurrentSerialPort->clear();

    mCachedSeralPortInfoList = QSerialPortInfo::availablePorts();
    foreach(const QSerialPortInfo &portInfo, QSerialPortInfo::availablePorts()) {
        ui->serialPortListWidget->addItem(portInfo.portName());
    }

//    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
//        qDebug() << "   Name=" << info.portName()
//                 << " Description=" << info.description()
//                 << " Manufacturer=" << info.manufacturer();
//        // Example use QSerialPort
////        QSerialPort serial;
////        serial.setPort(info);
////        if (serial.open(QIODevice::ReadWrite))
////            serial.close();
//    }
    appendLog("查找串口");
}

void MainWindow::on_startNetInButton_clicked() {
    appendLog("开启入网");
}

void MainWindow::on_terminalSignInButton_clicked() {
    appendLog("终端签到");
}

void MainWindow::on_removeTerminalButton_clicked() {
    appendLog("删除终端");
}

void MainWindow::on_modifyPanIDButton_clicked() {
    appendLog("修改PAN ID");
}

void MainWindow::on_closeNetButton_clicked() {
    appendLog("关闭入网");
}

void MainWindow::on_readCalliperDataButton_clicked() {
    appendLog("读取卡尺数据");
}

void MainWindow::on_openSerialPortButton_clicked() {
    if(mCurrentSerialPort.isOpen()) {
        appendMessageTipAndRecordLog("串口 " + mCurrentSerialPort.portName() + " 已打开，请先关闭！");
        return;
    }

    QList<QListWidgetItem*> selectedItems = ui->serialPortListWidget->selectedItems();
    if(selectedItems.size() <= 0) {
        appendMessageTipAndRecordLog("请先选择需要打开的串口！");
        return;
    }

    QListWidgetItem *selItem = selectedItems.at(0);
    int selRowIndex = ui->serialPortListWidget->row(selItem);
    QSerialPortInfo selPortInfo = mCachedSeralPortInfoList.at(selRowIndex);
    mCurrentSerialPort.setPort(selPortInfo);
    bool result = mCurrentSerialPort.open(QIODevice::ReadWrite);
//    QString portName = selectedItems.indexOf(selItem)
    QString resultTip = (result ? "成功" : "失败");
    appendLog("打开串口 " + selPortInfo.portName() + resultTip);
}

void MainWindow::on_closeSerialPortButton_clicked() {
    if(mCurrentSerialPort.isOpen()) {
        mCurrentSerialPort.close();
        appendLog("关闭串口 " + mCurrentSerialPort.portName());
    }

}

void MainWindow::appendLog(const QString &logText) {
    QDateTime currentDateTime = QDateTime::currentDateTime();
//    QString currentDateTimeStr = currentDateTime.toString("yyyy.MM.dd hh:mm:ss ");
    QString currentDateTimeStr = currentDateTime.toString("hh:mm:ss ");
    ui->logTextBrowser->append(currentDateTimeStr + logText);
}

void MainWindow::appendMessageTipAndRecordLog(const QString &tipText) {
    QMessageBox::information(NULL, "提醒", tipText);
    appendLog(tipText);
}
