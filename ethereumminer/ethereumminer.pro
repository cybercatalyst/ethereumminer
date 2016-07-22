QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ethereumminer
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS += mainwindow.h

FORMS += mainwindow.ui

include(../pods.pri)

RESOURCES += \
    resources.qrc
