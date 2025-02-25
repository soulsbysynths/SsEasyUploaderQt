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
    QComboBox *cboCommPort;
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
        MainWindow->resize(291, 310);
        QIcon icon;
        icon.addFile(QString::fromUtf8("my_app.icns"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        MainWindow->setWindowIcon(icon);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName("centralWidget");
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(10, 10, 271, 234));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(7);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setSizeConstraint(QLayout::SizeConstraint::SetDefaultConstraint);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        cboCommPort = new QComboBox(layoutWidget);
        cboCommPort->setObjectName("cboCommPort");

        verticalLayout->addWidget(cboCommPort);

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
        horizontalLayoutWidget->setGeometry(QRect(10, 250, 272, 32));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        btnSavePatches = new QPushButton(horizontalLayoutWidget);
        btnSavePatches->setObjectName("btnSavePatches");

        horizontalLayout->addWidget(btnSavePatches);

        btnLoadPatches = new QPushButton(horizontalLayoutWidget);
        btnLoadPatches->setObjectName("btnLoadPatches");

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
        btnUploadFlash->setText(QCoreApplication::translate("MainWindow", "Upload...", nullptr));
        lblOutput->setText(QCoreApplication::translate("MainWindow", "1) Select Comm Port 2) Click Upload... 3) Select file", nullptr));
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
