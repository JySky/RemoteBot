#-------------------------------------------------
#
# Project created by QtCreator 2016-02-24T21:12:43
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += widgets
QT       += webkit
QT       += webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Wifibot
TEMPLATE = app


SOURCES += main.cpp\
        interface.cpp \
    clientcontrol.cpp \
    config.cpp \
    cameratraitement.cpp \
    clientreceivetraitement.cpp \
    clientcamera.cpp

HEADERS  += interface.h \
    clientcontrol.h \
    config.h \
    cameratraitement.h \
    clientreceivetraitement.h \
    clientcamera.h

FORMS    += interface.ui \
    config.ui
