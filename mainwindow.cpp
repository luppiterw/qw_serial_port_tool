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

//    QByteArray byteArray;
//    byteArray.append(0x5A);
 //    byteArray.append(0xA5);

//    signed char
//    char cmd[6];
//    cmd[0] = 0x5A;
//    cmd[1] = 0xA5;  ///< 0、1为帧头
//    cmd[4] = 0x44;  ///< 功能码-开启入网（下行）

    unsigned char cmd[6] = {
        0x5A, 0xA5, 0x0, 0x0, 0x44
    };

//    qDebug() << "MainWindow::on_startNetInButton_clicked cmd="<<cmd;

    if(mCurrentSerialPort.isOpen()) {
        qDebug() << "MainWindow::on_startNetInButton_clicked write 0x5A_0xA5_-_-_0x44" << mCurrentSerialPort.write(reinterpret_cast<const char*>(cmd));
//        qDebug() << "mCurrentSerialPort.write 0x5A_0xA5_-_-_0x44" << mCurrentSerialPort.write(cmd);
        appendLog("开启入网 下行 0x5A_0xA5_-_-_0x44");
    }
}

void MainWindow::on_terminalSignInButton_clicked() {
    unsigned char cmd[6] = {
        0x5A, 0xA5, 0x0, 0x0, 0x66
    };
//    cmd[0] = 0x5A;
//    cmd[1] = 0xA5;  ///< 0、1为帧头
//    cmd[4] = 0x66;  ///< 功能码-终端签到（下行）

    if(mCurrentSerialPort.isOpen()) {
        qDebug() << "MainWindow::on_terminalSignInButton_clicked write 0x5A_0xA5_-_-_0x66" << mCurrentSerialPort.write(reinterpret_cast<const char*>(cmd));
        appendLog("终端签到 下行 0x5A_0xA5_-_-_0x66");
    }
}

void MainWindow::on_removeTerminalButton_clicked() {
    ///< todo
    appendLog("删除终端");
}

void MainWindow::on_modifyPanIDButton_clicked() {
    ///< todo
    appendLog("修改PAN ID");
}

void MainWindow::on_closeNetButton_clicked() {
    unsigned char cmd[6] = {
        0x5A, 0xA5, 0x0, 0x0, 0x44
    };

    if(mCurrentSerialPort.isOpen()) {
        qDebug() << "MainWindow::on_startNetInButton_clicked write 0x5A_0xA5_-_-_0x33" << mCurrentSerialPort.write(reinterpret_cast<const char*>(cmd));
        appendLog("关闭入网 下行 0x5A_0xA5_-_-_0x33");
    }
}

void MainWindow::on_readCalliperDataButton_clicked() {

    qDebug() << "MainWindow::on_readCalliperDataButton_clicked mCurrentSerialPort.bytesAvailable()=" <<mCurrentSerialPort.bytesAvailable();

    if(mCurrentSerialPort.bytesAvailable() > 0) {
        QByteArray buffer = mCurrentSerialPort.readAll();
        ///< 读取端口数据并显示
        ui->calliperDataLabel->setText(buffer);
        appendLog("读取卡尺数据，内容为 " + buffer);
    } else {
        appendLog("读取卡尺数据，数据长度=" + QString::number(mCurrentSerialPort.bytesAvailable()));
    }
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
    ///< 绑定QIODevice::readyRead 信号
    connect(&mCurrentSerialPort, &QIODevice::readyRead, this, &MainWindow::readComDataByReadyReadSignal);

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

void MainWindow::readComDataByReadyReadSignal() {
    qDebug() << "MainWindow::readComDataByReadyReadSignal";
    if(mCurrentSerialPort.isOpen()) {
        QByteArray byteArray = mCurrentSerialPort.readAll();
        appendLog("获取到readyRead信号数据 " + byteArray);
        ui->autoCalliperDataLabel->setText(byteArray);
    }
}
