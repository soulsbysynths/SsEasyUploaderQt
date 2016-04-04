#-------------------------------------------------
#
# Project created by QtCreator 2016-03-09T18:05:05
#
#-------------------------------------------------



QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EasyUploader
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui


macx:ICON = $${PWD}/icon.icns
win32:RC_ICONS = icon.ico

