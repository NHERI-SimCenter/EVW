#-------------------------------------------------
#
# Project created by QtCreator 2017-03-11T14:00:13
#
#-------------------------------------------------

QT       += core gui printsupport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EVW
TEMPLATE = app

include(OPS_includes.pri)
include(Wind_includes.pri)
include(../SimCenterCommon/Common/Common.pri)

VERSION=1.1.1
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

SOURCES += main.cpp\
        MainWindow.cpp \
        MyGlWidget.cpp \
        qcustomplot.cpp \
        EarthquakeRecord.cpp \
        SimpleSpreadsheetWidget.cpp \
        ResponseWidget.cpp

HEADERS  += MainWindow.h \
    MyGlWidget.h \
    qcustomplot.h \
    EarthquakeRecord.h \
    SimpleSpreadsheetWidget.h \
    ResponseWidget.h

RESOURCES += \
    images.qrc \
    mdof.gif
