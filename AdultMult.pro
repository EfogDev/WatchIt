#-------------------------------------------------
#
# Project created by QtCreator 2015-01-01T21:13:23
#
#-------------------------------------------------

QT       += core gui webkit webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AdultMult
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serial.cpp

HEADERS  += mainwindow.h \
    const.h \
    serial.h

DISTFILES += \
    ListWidget.css

RESOURCES += \
    styles.qrc
