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

win32:VERSION = 1.2.1.0 # major.minor.patch.build
else:VERSION = 1.2.1    # major.minor.patch

macx {
    QMAKE_POST_LINK += macdeployqt $$OUT_PWD/build/bin/$${TARGET}.app
}

DISTFILES += \
    Copying.txt \
    avrdude.exe \
    avrdude.conf \
    EEPROM_Reader.hex \
    EEPROM_writer.hex

