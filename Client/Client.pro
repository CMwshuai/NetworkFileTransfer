#-------------------------------------------------
#
# Project created by QtCreator 2018-08-01T11:57:10
#
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h \
    debugonoroff.h \
    protocolcommand.h

FORMS    += widget.ui

RESOURCES += \
    document.qrc
