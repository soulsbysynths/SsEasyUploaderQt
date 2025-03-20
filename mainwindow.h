// Easy Uploader. AVRDUDE interface for use with Soulsby Atmegatron.
// Copyright (C) 2016 Paul Soulsby

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QProcess>
#include <QSerialPort>
#include <QSettings>
#include <QStandardPaths>
#include <QTextStream>
#include <QThread>
#include <QTimer>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui
{
class easyUploaderDevice;
class MainWindow;
} // namespace Ui

typedef struct easyUploaderDevice
{
    QString name;
    QString preUploadMsg;
    QString preUploadProcess1;
    QStringList preUploadArgs1;
    QString preUploadProcess2;
    QStringList preUploadArgs2;
    QStringList avrDudeArgs;
} easyUploaderDevice;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnUploadFlash_clicked();
    void on_avrOutput();
    void on_avrFinished(int data, QProcess::ExitStatus status);
    void serialReceived();
    void on_chkShowConsole_stateChanged(int arg1);
    void on_cboCommPort_activated(int index);
    void on_btnSavePatches_clicked();
    void on_btnLoadPatches_clicked();
    void on_timerTimeout();

    void on_cboModule_activated(int index);

private:
    enum Task
    {
        T_IDLE,
        T_UPLOAD,
        T_SP_DL_FW,
        T_SP_UL_EEP,
        T_SP_SAVE_EEP,
        T_SP_UL_FW,
        T_LP_DL_FW,
        T_LP_UL_EEP,
        T_LP_LOAD_EEP,
        T_LP_UL_FW
    };
    static const int DEVICES = 4;
    easyUploaderDevice devices[DEVICES] = {
        {
            "Bread & Butter",
            "",
#ifdef Q_OS_WINDOWS
            "mode.com",
            {
                "COM:",
                "baud=1200",
                "dtr=on"
            },
            "mode.com",
            {
                "COM:",
                "dtr=off"
            },
#else
            "stty",
            {
                "-f",
                "COM",
                "1200"
            },
            "",
            {},
#endif
            {
                "-C",
                "",
                "-v",
                "-V",
                "-p",
                "atmega4809",
                "-c",
                "jtag2updi",
                "-P",
                "",
                "-b",
                "115200",
                "-e",
                "-D",
                "-U",
                "",
                "-U",
                "fuse2:w:0x01:m",
                "-U",
                "fuse5:w:0xC9:m",
                "-U",
                "fuse8:w:0x00:m",
                "{upload.extra_files}"
            }
        },

        {
            "Euclidean (Rev 1.1 & 1.2)",
            "",
            "",
            {},
            "",
            {},
            {
                "-C",
                "",
                "-v",
                "-V",
                "-p",
                "atmega328p",
                "-c",
                "arduino",
                "-P",
                "",
                "-b",
                "57600",
                "-D",
                "-U",
                ""
            }
        },

        {
            "Euclidean (Rev 1.3+)",
            "",
#ifdef Q_OS_WINDOWS
            "mode.com",
            {
                "COM:",
                "baud=1200",
                "dtr=on"
            },
            "mode.com",
            {
                "COM:",
                "dtr=off"
            },
#else
            "stty",
            {
                "-f",
                "COM",
                "1200"
            },
            "",
            {},
#endif
            {
                "-C",
                "",
                "-v",
                "-V",
                "-p",
                "atmega4809",
                "-c",
                "jtag2updi",
                "-P",
                "",
                "-b",
                "115200",
                "-e",
                "-D",
                "-U",
                "",
                "-U",
                "fuse2:w:0x01:m",
                "-U",
                "fuse5:w:0xC9:m",
                "-U",
                "fuse8:w:0x00:m",
                "{upload.extra_files}"
            }
        },

        {
            "Odds",
            "Please press the reset switch on the Adafruit Pro Trinket 5V before uploading the firmware. Make sure that the red LED is fading in and out when you click the OK button.",
            "",
            {},
            "",
            {},
            {
                "-C",
                "",
                "-v",
                "-p",
                "atmega328p",
                "-c",
                "usbtiny",
                "-U",
                ""
            }
        },
        //         {
        //             "Atmegatron",
        //             "This is a test pre-upload message.",
        // #ifdef Q_OS_WINDOWS
        //             "mode.com",
        //             {
        //                 "COM:",
        //                 "baud=1200",
        //                 "dtr=on"
        //             },
        //             "mode.com",
        //             {
        //                 "COM:",
        //                 "dtr=off"
        //             },
        // #else
        //             "stty",
        //             {
        //                 "-f",
        //                 "COM",
        //                 "1200"
        //             },
        //             "",
        //             {},
        // #endif
        //             {
        //                 "-c",
        //                 "arduino",
        //                 "-p",
        //                 "m328p",
        //                 "-C",
        //                 "",
        //                 "-P",
        //                 "",
        //                 "-U",
        //                 ""
        //             }
        //         },
    };


    Ui::MainWindow *ui;
    QSettings *settings;
    QProcess *avrprog;
    QSerialPort *serial;
    QTimer *timer;
    void populateCombo();
    void enableButtons(bool way);
    bool fileExists(QString path);
    void showConsole(bool way);
    Task curTask = T_IDLE;
    unsigned char curEepromBlock = 0;
    QByteArray serialDataRx;
    void backupFlash();
    const unsigned char BLOCKS = 64;
    void callAvrdude(QString hexPath, bool write);
    QString atmBackUpFilepath();
    bool isConsoleContainsCompleteWord(bool write);
    void printQProcessToConsole(QString prog, QStringList args);
    void runPreUploadProcess(QString process, QStringList args);
};

#endif // MAINWINDOW_H
