#-------------------------------------------------
#
# Project created by QtCreator 2016-02-24T21:12:43
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Wifibot
TEMPLATE = app


SOURCES += main.cpp\
        interface.cpp \
    clientcontrol.cpp

HEADERS  += interface.h \
    clientcontrol.h

FORMS    += interface.ui
