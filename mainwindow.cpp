#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QFileDialog>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings(new QSettings("Soulsby Synthesizers", "Easy Uploader"))
{
    ui->setupUi(this);
    ui->txtOutput->setVisible(false);
    this->setFixedSize(this->size());

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timer_timeout()));

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
    connect(avrprog, SIGNAL(readyReadStandardOutput()), this, SLOT(avrOutput()));
    connect(avrprog, SIGNAL(finished(int , QProcess::ExitStatus )), this, SLOT(avrFinished(int , QProcess::ExitStatus )));

    populateCombo();
    int index = ui->cboCommPort->findText(settings->value("commPort").toString());
    ui->cboCommPort->setCurrentIndex(index);
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
        QString prog =  "\"";
        prog.append(qApp->applicationDirPath());
        prog.append("//avrdude\" -c arduino -p m328p -C \"");
        prog.append(qApp->applicationDirPath());
        prog.append("/avrdude.conf\" -P ");
        prog.append(settings->value("commPort").toString());
        prog.append(" -U flash:w:\"");
        prog.append(filename);
        prog.append("\":i");
        avrprog->start(prog);
        enableButtons(false);
        ui->txtOutput->clear();
        ui->txtOutput->insertPlainText(prog);
        ui->txtOutput->insertPlainText("\n");
        ui->lblOutput->setText("Programming. Please wait...");
    }

}
void MainWindow::enableButtons(bool way)
{
    ui->btnUploadFlash->setEnabled(way);
    ui->cboCommPort->setEnabled(way);
    ui->btnLoadPatches->setEnabled(way);
    ui->btnSavePatches->setEnabled(way);
}

void MainWindow::avrOutput()
{
    QByteArray output = avrprog->readAllStandardOutput();
    ui->txtOutput->insertPlainText(output);
    ui->txtOutput->ensureCursorVisible();
}
void MainWindow::avrFinished(int data, QProcess::ExitStatus status)
{
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
        if(ui->txtOutput->toPlainText().contains("writing output file")==true)
        {
            ui->lblOutput->setText("Uploading EEPROM reader. Please wait...");
            curTask = T_SP_UL_EEP;
            QString prog =  "\"";
            prog.append(qApp->applicationDirPath());
            prog.append("//avrdude\" -c arduino -p m328p -C \"");
            prog.append(qApp->applicationDirPath());
            prog.append("/avrdude.conf\" -P ");
            prog.append(settings->value("commPort").toString());
            prog.append(" -U flash:w:\"");
            prog.append(qApp->applicationDirPath());
            prog.append("/EEPROM_Reader.hex");
            prog.append("\":i");
            avrprog->start(prog);
            ui->txtOutput->clear();
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
        if(ui->txtOutput->toPlainText().contains("writing output file")==true)
        {
            ui->lblOutput->setText("Uploading EEPROM writer. Please wait...");
            curTask = T_LP_UL_EEP;
            QString prog =  "\"";
            prog.append(qApp->applicationDirPath());
            prog.append("//avrdude\" -c arduino -p m328p -C \"");
            prog.append(qApp->applicationDirPath());
            prog.append("/avrdude.conf\" -P ");
            prog.append(settings->value("commPort").toString());
            prog.append(" -U flash:w:\"");
            prog.append(qApp->applicationDirPath());
            prog.append("/EEPROM_writer.hex");
            prog.append("\":i");
            avrprog->start(prog);
            ui->txtOutput->clear();
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
                    QString prog =  "\"";
                    prog.append(qApp->applicationDirPath());
                    prog.append("//avrdude\" -c arduino -p m328p -C \"");
                    prog.append(qApp->applicationDirPath());
                    prog.append("/avrdude.conf\" -P ");
                    prog.append(settings->value("commPort").toString());
                    prog.append(" -U flash:w:\"");
                    prog.append(qApp->applicationDirPath());
                    prog.append("/atm_backup.hex\":i");
                    avrprog->start(prog);
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
            QString prog =  "\"";
            prog.append(qApp->applicationDirPath());
            prog.append("//avrdude\" -c arduino -p m328p -C \"");
            prog.append(qApp->applicationDirPath());
            prog.append("/avrdude.conf\" -P ");
            prog.append(settings->value("commPort").toString());
            prog.append(" -U flash:w:\"");
            prog.append(qApp->applicationDirPath());
            prog.append("/atm_backup.hex\":i");
            avrprog->start(prog);
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

void MainWindow::on_timer_timeout()
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
    if(filename!=NULL)
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
        settings->setValue("lpFilename",filename);
        curTask = T_LP_DL_FW;
        backupFlash();
    }
}
void MainWindow::backupFlash()
{
    serial->setPortName(settings->value("commPort").toString());
    QString prog =  "\"";
    prog.append(qApp->applicationDirPath());
    prog.append("//avrdude\" -c arduino -p m328p -C \"");
    prog.append(qApp->applicationDirPath());
    prog.append("/avrdude.conf\" -P ");
    prog.append(settings->value("commPort").toString());
    prog.append(" -U flash:r:\"");
    prog.append(qApp->applicationDirPath());
    prog.append("/atm_backup.hex\":i");
    avrprog->start(prog);
    enableButtons(false);
    ui->txtOutput->clear();
    ui->txtOutput->insertPlainText(prog);
    ui->txtOutput->insertPlainText("\n");
    ui->lblOutput->setText("Backing up flash. Please wait...");
}
