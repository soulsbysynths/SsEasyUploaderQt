/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *lblInstruction1;
    QComboBox *cboModule;
    QLabel *lblInstruction1_2;
    QComboBox *cboCommPort;
    QLabel *lblInstruction1_3;
    QPushButton *btnUploadFlash;
    QLabel *lblOutput;
    QCheckBox *chkShowConsole;
    QTextEdit *txtOutput;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnSavePatches;
    QPushButton *btnLoadPatches;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(294, 385);
        QIcon icon;
        icon.addFile(QString::fromUtf8("my_app.icns"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        MainWindow->setWindowIcon(icon);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName("centralWidget");
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(10, 10, 271, 351));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(7);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setSizeConstraint(QLayout::SizeConstraint::SetMinimumSize);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        lblInstruction1 = new QLabel(layoutWidget);
        lblInstruction1->setObjectName("lblInstruction1");
        lblInstruction1->setFrameShape(QFrame::Shape::NoFrame);
        lblInstruction1->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        lblInstruction1->setWordWrap(true);
        lblInstruction1->setMargin(0);
        lblInstruction1->setIndent(0);

        verticalLayout->addWidget(lblInstruction1);

        cboModule = new QComboBox(layoutWidget);
        cboModule->setObjectName("cboModule");

        verticalLayout->addWidget(cboModule);

        lblInstruction1_2 = new QLabel(layoutWidget);
        lblInstruction1_2->setObjectName("lblInstruction1_2");
        lblInstruction1_2->setFrameShape(QFrame::Shape::NoFrame);
        lblInstruction1_2->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        lblInstruction1_2->setWordWrap(true);
        lblInstruction1_2->setMargin(0);
        lblInstruction1_2->setIndent(0);

        verticalLayout->addWidget(lblInstruction1_2);

        cboCommPort = new QComboBox(layoutWidget);
        cboCommPort->setObjectName("cboCommPort");

        verticalLayout->addWidget(cboCommPort);

        lblInstruction1_3 = new QLabel(layoutWidget);
        lblInstruction1_3->setObjectName("lblInstruction1_3");
        lblInstruction1_3->setFrameShape(QFrame::Shape::NoFrame);
        lblInstruction1_3->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        lblInstruction1_3->setWordWrap(true);
        lblInstruction1_3->setMargin(0);
        lblInstruction1_3->setIndent(0);

        verticalLayout->addWidget(lblInstruction1_3);

        btnUploadFlash = new QPushButton(layoutWidget);
        btnUploadFlash->setObjectName("btnUploadFlash");

        verticalLayout->addWidget(btnUploadFlash);

        lblOutput = new QLabel(layoutWidget);
        lblOutput->setObjectName("lblOutput");
        lblOutput->setFrameShape(QFrame::Shape::NoFrame);
        lblOutput->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        lblOutput->setWordWrap(true);
        lblOutput->setMargin(0);
        lblOutput->setIndent(0);

        verticalLayout->addWidget(lblOutput);

        chkShowConsole = new QCheckBox(layoutWidget);
        chkShowConsole->setObjectName("chkShowConsole");
        chkShowConsole->setMinimumSize(QSize(0, 28));

        verticalLayout->addWidget(chkShowConsole);

        txtOutput = new QTextEdit(layoutWidget);
        txtOutput->setObjectName("txtOutput");
        txtOutput->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
        txtOutput->setReadOnly(true);

        verticalLayout->addWidget(txtOutput);

        horizontalLayoutWidget = new QWidget(centralWidget);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(10, 380, 272, 32));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        btnSavePatches = new QPushButton(horizontalLayoutWidget);
        btnSavePatches->setObjectName("btnSavePatches");
        btnSavePatches->setEnabled(false);

        horizontalLayout->addWidget(btnSavePatches);

        btnLoadPatches = new QPushButton(horizontalLayoutWidget);
        btnLoadPatches->setObjectName("btnLoadPatches");
        btnLoadPatches->setEnabled(false);

        horizontalLayout->addWidget(btnLoadPatches);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName("statusBar");
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QString());
        lblInstruction1->setText(QCoreApplication::translate("MainWindow", "Select which module to update:", nullptr));
        lblInstruction1_2->setText(QCoreApplication::translate("MainWindow", "Select the correct serial port:", nullptr));
        lblInstruction1_3->setText(QCoreApplication::translate("MainWindow", "Locate the .hex file and upload it:", nullptr));
        btnUploadFlash->setText(QCoreApplication::translate("MainWindow", "Upload...", nullptr));
        lblOutput->setText(QString());
        chkShowConsole->setText(QCoreApplication::translate("MainWindow", "Show Console", nullptr));
        btnSavePatches->setText(QCoreApplication::translate("MainWindow", "Save Patches...", nullptr));
        btnLoadPatches->setText(QCoreApplication::translate("MainWindow", "Load Patches...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
