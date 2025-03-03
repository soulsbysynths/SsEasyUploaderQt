//Easy Uploader. AVRDUDE interface for use with Soulsby Atmegatron.
//Copyright (C) 2016 Paul Soulsby

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings(new QSettings("Soulsby Synthesizers", "Easy Uploader"))
{
    ui->setupUi(this);
    ui->txtOutput->setVisible(false);
    this->setFixedSize(this->size());

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timerTimeout()));

    serial = new QSerialPort(this);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    connect(serial,SIGNAL(readyRead()),this,SLOT(serialReceived()));

    avrprog = new QProcess(this);
    avrprog->setProcessChannelMode(QProcess::MergedChannels);
    avrprog->setReadChannel(QProcess::StandardOutput);
    connect(avrprog, SIGNAL(readyReadStandardOutput()), this, SLOT(on_avrOutput()));
    connect(avrprog, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(on_avrFinished(int,QProcess::ExitStatus)));
    connect(avrprog, &QProcess::errorOccurred, [=](QProcess::ProcessError error)
    {
        qDebug() << "error enum val = " << error << "\n" << avrprog->errorString() << "\n";
    });

    populateCombo();
    int index = ui->cboCommPort->findText(settings->value("commPort").toString());
    ui->cboCommPort->setCurrentIndex(index);
    this->setWindowTitle("Easy Uploader " + QApplication::applicationVersion());
}
void MainWindow::populateCombo()
{
    ui->cboCommPort->clear();
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        ui->cboCommPort->addItem(serialPortInfo.systemLocation());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete settings;
    delete avrprog;
}
bool MainWindow::fileExists(QString path) {
    QFileInfo checkFile(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (checkFile.exists() && checkFile.isFile()) {
        return true;
    } else {
        return false;
    }
}
void MainWindow::on_btnUploadFlash_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,"Upload", settings->value("filename").toString() ,"Hex files (*.hex)");
    if(fileExists(filename)==true)
    {
        curTask = T_UPLOAD;
        settings->setValue("filename",filename);
        ui->txtOutput->clear();
        callAvrdude(filename,true);
        enableButtons(false);
        ui->lblOutput->setText("Programming. Please wait...");
    }

}
void MainWindow::callAvrdude(QString hexPath, bool write)
{
    QString fileMode;
    if(write)
    {
        fileMode = "w";
    }
    else
    {
        fileMode = "r";
    }
    QString prog = qApp->applicationDirPath() + "//avrdude";
    QStringList args = {"-c","arduino","-p","m328p","-C", qApp->applicationDirPath() + "/avrdude.conf","-P",settings->value("commPort").toString(),"-U","flash:" + fileMode + ":"+ hexPath + ":i"};
    avrprog->start(prog,args);
    ui->txtOutput->insertPlainText(prog);
    for(int i=0;i<args.length();++i )
    {
        ui->txtOutput->insertPlainText(" " + args.at(i));
    }
    ui->txtOutput->insertPlainText("\n");
}
void MainWindow::enableButtons(bool way)
{
    ui->btnUploadFlash->setEnabled(way);
    ui->cboCommPort->setEnabled(way);
    ui->btnLoadPatches->setEnabled(way);
    ui->btnSavePatches->setEnabled(way);
}

void MainWindow::on_avrOutput()
{
    QByteArray output = avrprog->readAllStandardOutput();
    ui->txtOutput->insertPlainText(output);
    ui->txtOutput->ensureCursorVisible();
}
void MainWindow::on_avrFinished(int data, QProcess::ExitStatus status)
{
    if(status==QProcess::CrashExit)
    {
        ui->txtOutput->append("Crashed");
    }
    else
    {
        ui->txtOutput->append("Success:" + QString::number(data));
    }
    switch(curTask)
    {
    case T_UPLOAD:
        if(ui->txtOutput->toPlainText().contains("flash verified")==true)
        {
            ui->lblOutput->setText("Completed!");
        }
        else
        {
            ui->lblOutput->setText("Failed");
        }
        enableButtons(true);
        curTask = T_IDLE;
        break;
    case T_SP_DL_FW:
        if(ui->txtOutput->toPlainText().contains("output file")==true)
        {
            ui->lblOutput->setText("Uploading EEPROM reader. Please wait...");
            curTask = T_SP_UL_EEP;
            ui->txtOutput->clear();
            callAvrdude(qApp->applicationDirPath() + "/EEPROM_Reader.hex",true);
        }
        else
        {
            ui->lblOutput->setText("Failed");
            enableButtons(true);
        }
        break;
    case T_SP_UL_EEP:
        if(ui->txtOutput->toPlainText().contains("flash verified")==true)
        {
            ui->lblOutput->setText("Transferring EEPROM. Please wait...");
            curTask = T_SP_SAVE_EEP;
            if(serial->open(QIODevice::ReadWrite))
            {
                serialDataRx.clear();
                timer->start(500);
            }
            else
            {
                ui->lblOutput->setText("Failed to open serial port");
                enableButtons(true);
                curTask = T_IDLE;
            }
        }
        else
        {
            ui->lblOutput->setText("Failed");
            enableButtons(true);
            curTask = T_IDLE;
        }
        break;
    case T_SP_UL_FW:
    case T_LP_UL_FW:
        if(ui->txtOutput->toPlainText().contains("flash verified")==true)
        {
            ui->lblOutput->setText("Completed!");
        }
        else
        {
            ui->lblOutput->setText("Failed");
        }
        enableButtons(true);
        curTask = T_IDLE;
        break;
    case T_LP_DL_FW:
        if(ui->txtOutput->toPlainText().contains("output file")==true)
        {
            ui->lblOutput->setText("Uploading EEPROM writer. Please wait...");
            curTask = T_LP_UL_EEP;
            ui->txtOutput->clear();
            callAvrdude(qApp->applicationDirPath() + "/EEPROM_writer.hex", true);
        }
        else
        {
            ui->lblOutput->setText("Failed");
            enableButtons(true);
        }
        break;
    case T_LP_UL_EEP:
        if(ui->txtOutput->toPlainText().contains("flash verified")==true)
        {
            ui->lblOutput->setText("Transferring EEPROM. Please wait...");
            curTask = T_LP_LOAD_EEP;
            if(serial->open(QIODevice::ReadWrite))
            {
                serialDataRx.clear();
                ui->txtOutput->clear();
            }
            else
            {
                ui->lblOutput->setText("Failed to open serial port");
                enableButtons(true);
                curTask = T_IDLE;
            }
        }
        else
        {
            ui->lblOutput->setText("Failed");
            enableButtons(true);
            curTask = T_IDLE;
        }
        break;
    default:
        break;
    }
}
void MainWindow::serialReceived()
{
    QString line;
    timer->stop();
    serialDataRx.append(serial->readAll());
    if(curTask==T_SP_SAVE_EEP)
    {
        if(serialDataRx.size()>=21)
        {
            QString out = ":";
            out.append(QString(serialDataRx.toHex()));
            out = out.toUpper();
            ui->txtOutput->append(out);
            if(curEepromBlock<63)
            {
                curEepromBlock++;
                serialDataRx.clear();
                QByteArray data;
                data.append(curEepromBlock);
                serial->write(data);
            }
            else
            {
                serial->close();
                ui->txtOutput->append(":00000001FF");
                QFile file(settings->value("spFilename").toString());
                if (file.open(QIODevice::ReadWrite))
                {
                    QTextStream stream(&file);
                    stream << ui->txtOutput->toPlainText();
                    file.flush();
                    file.close();
                    ui->txtOutput->clear();
                    ui->lblOutput->setText("Restoring flash. Please wait...");
                    curTask = T_SP_UL_FW;
                    //callAvrdude(qApp->applicationDirPath() + "/atm_backup.hex",true);
                    callAvrdude(atmBackUpFilepath(),true);
                }
                else
                {
                    ui->lblOutput->setText("Failed. Could not write to file.");
                    serial->close();
                    curTask = T_IDLE;
                    enableButtons(true);
                }

            }
        }
    }
    else if(curTask==T_LP_LOAD_EEP)
    {
        curEepromBlock = serialDataRx[0];
        serialDataRx.clear();
        if(curEepromBlock==0xFF)
        {
            ui->txtOutput->append("First byte error");
            ui->lblOutput->setText("WARNING: write error");
        }
        else if (curEepromBlock==0xFE)
        {
            ui->txtOutput->append("Checksum error");
            ui->lblOutput->setText("WARNING: write error");
        }
        else if(curEepromBlock==BLOCKS)
        {
            serial->close();
            ui->txtOutput->clear();
            ui->lblOutput->setText("Restoring flash. Please wait...");
            curTask = T_SP_UL_FW;
            callAvrdude(atmBackUpFilepath(), true);
        }
        else
        {
            QFile file(settings->value("lpFilename").toString());
            if (file.open(QIODevice::ReadWrite))
            {
                QTextStream stream(&file);
                for(unsigned char i=0;i<=curEepromBlock;++i)
                {
                    line = stream.readLine(0);
                }
                file.flush();
                file.close();
                if(line.size()==0)
                {
                    ui->lblOutput->setText("Failed. Could not read file.");
                    serial->close();
                    curTask = T_IDLE;
                    enableButtons(true);
                }
                else
                {
                    ui->txtOutput->append(line);
                    QByteArray outdata;
                    for(unsigned char i=1;i<43;i+=2)
                    {
                        QString pair = line.mid(i,2);
                        //ui->txtOutput->append(pair);
                        bool ok;
                        char val = pair.toInt(&ok,16);
                        //ui->txtOutput->append(QString::number(val));
                        outdata.append(val);
                        if(ok==false)
                        {
                            ui->txtOutput->append("write error");
                        }
                    }
                    serial->write(outdata);
                }
            }
            else
            {
                ui->lblOutput->setText("Failed. Could not open file.");
                serial->close();
                curTask = T_IDLE;
                enableButtons(true);
            }
        }

    }

}

QString MainWindow::atmBackUpFilepath()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QDir dir(path);
    if (!dir.exists())
    {
        dir.mkpath(".");
    }

    return path + "/atm_backup.hex";
}

void MainWindow::on_timerTimeout()
{
    QByteArray data;
    if(curTask==T_SP_SAVE_EEP)
    {
        curEepromBlock = 0;
        data.append(curEepromBlock);
        serial->write(data);
        ui->txtOutput->clear();
    }
}

void MainWindow::on_chkShowConsole_stateChanged(int arg1)
{
    if(arg1==Qt::Checked)
    {
        ui->txtOutput->setVisible(true);
    }
    else
    {
        ui->txtOutput->setVisible(false);
    }
}

void MainWindow::on_cboCommPort_activated(int index)
{
    settings->setValue("commPort",ui->cboCommPort->itemText(index));
}
//test commit

void MainWindow::on_btnSavePatches_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,"Save Patches",settings->value("spFilename").toString(),"EEPROM files (*.eep)");
    if(filename.isNull()==false && filename.isEmpty()==false)
    {
        settings->setValue("spFilename",filename);
        curTask = T_SP_DL_FW;
        backupFlash();
    }
}


void MainWindow::on_btnLoadPatches_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,"Load Patches", settings->value("lpFilename").toString() ,"EEPROM files (*.eep)");
    if(fileExists(filename)==true)
    {
        if(QMessageBox::question(this, "Load Patches"  ,  "This will erase all existing patches in your device. OK to continue?", QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
        {
            settings->setValue("lpFilename",filename);
            curTask = T_LP_DL_FW;
            backupFlash();
        }
    }
}
void MainWindow::backupFlash()
{
    serial->setPortName(settings->value("commPort").toString());
    ui->txtOutput->clear();
    callAvrdude(atmBackUpFilepath() ,false);
    enableButtons(false);
    ui->lblOutput->setText("Backing up flash. Please wait...");
}
