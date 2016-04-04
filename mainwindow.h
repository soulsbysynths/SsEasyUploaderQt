#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QApplication>
#include <QProcess>

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
    void on_chkShowConsole_stateChanged(int arg1);
    void on_cboCommPort_activated(int index);

private:
    Ui::MainWindow *ui;
    QSettings *settings;
    QProcess *avrprog;
    void populateCombo();
    bool fileExists(QString path);
};

#endif // MAINWINDOW_H
