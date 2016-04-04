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
    QString filename = QFileDialog::getOpenFileName(this,"Select .hex file", settings->value("filename").toString() ,"Hex files (*.hex)");
    if(fileExists(filename)==true)
    {
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
        ui->btnUploadFlash->setEnabled(false);
        ui->cboCommPort->setEnabled(false);
        ui->txtOutput->clear();
        ui->txtOutput->insertPlainText(prog);
        ui->txtOutput->insertPlainText("\n");
        ui->lblOutput->setText("Programming. Please wait...");
    }

}

void MainWindow::avrOutput()
{
    QByteArray output = avrprog->readAllStandardOutput();
    ui->txtOutput->insertPlainText(output);
    ui->txtOutput->ensureCursorVisible();
}
void MainWindow::avrFinished(int data, QProcess::ExitStatus status)
{
    if(ui->txtOutput->toPlainText().contains("flash verified")==true)
    {
        ui->lblOutput->setText("Completed!");
    }
    else
    {
        ui->lblOutput->setText("Failed");
    }
    ui->btnUploadFlash->setEnabled(true);
    ui->cboCommPort->setEnabled(true);
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
//test commit mac
