#-------------------------------------------------
#
# Project created by QtCreator 2015-01-01T21:13:23
#
#-------------------------------------------------

QT       += core gui widgets network webkit webkitwidgets

TARGET = AdultMult
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serial.cpp \
    helpwindow.cpp \
    settingswindow.cpp

HEADERS  += mainwindow.h \
    const.h \
    serial.h \
    helpwindow.h \
    settingswindow.h

DISTFILES += \
    ListWidget.css \
    browser.html

RESOURCES += \
    styles.qrc \
    web.qrc

CONFIG += C++11

FORMS += \
    helpwindow.ui \
    settingswindow.ui

RC_FILE = icon.rc
