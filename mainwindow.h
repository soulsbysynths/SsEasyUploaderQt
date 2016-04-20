#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QApplication>
#include <QProcess>
#include <QSerialPort>
#include <QTextStream>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnUploadFlash_clicked();
    void avrOutput();
    void avrFinished(int data , QProcess::ExitStatus status);
    void serialReceived();
    void on_chkShowConsole_stateChanged(int arg1);
    void on_cboCommPort_activated(int index);
    void on_btnSavePatches_clicked();
    void on_btnLoadPatches_clicked();
    void on_timer_timeout();
private:
    enum Task {T_IDLE,T_UPLOAD,T_SP_DL_FW,T_SP_UL_EEP,T_SP_SAVE_EEP,T_SP_UL_FW,T_LP_DL_FW,T_LP_UL_EEP,T_LP_LOAD_EEP,T_LP_UL_FW};
    Ui::MainWindow *ui;
    QSettings *settings;
    QProcess *avrprog;
    QSerialPort *serial;
    QTimer *timer;
    void populateCombo();
    void enableButtons(bool way);
    bool fileExists(QString path);
    Task curTask = T_IDLE;
    unsigned char curEepromBlock = 0;
    QByteArray serialDataRx;
    void backupFlash();
    const unsigned char BLOCKS = 64;
};

#endif // MAINWINDOW_H
