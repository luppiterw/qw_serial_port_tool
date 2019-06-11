#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_serialPortSearchButton_clicked();

    void on_startNetInButton_clicked();

    void on_terminalSignInButton_clicked();

    void on_removeTerminalButton_clicked();

    void on_modifyPanIDButton_clicked();

    void on_closeNetButton_clicked();

    void on_readCalliperDataButton_clicked();

    void on_openSerialPortButton_clicked();

    void on_closeSerialPortButton_clicked();

    ///< 根据readRead信号读取串口数据
    void readComDataByReadyReadSignal();
private:
    ///< 添加日志信息（含时间）
    void appendLog(const QString &logText);
    ///< 弹出提醒并添加到日志
    void appendMessageTipAndRecordLog(const QString &tipText);



private:
    QList<QSerialPortInfo> mCachedSeralPortInfoList;    ///< 每次点击查找串口之后缓存的串口信息

    QSerialPort mCurrentSerialPort;    ///<

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
