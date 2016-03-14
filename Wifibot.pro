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

win32: RC_ICONS = RemoteBot.ico
SOURCES += main.cpp\
        interface.cpp \
    clientcontrol.cpp \
    config.cpp \
    cameratraitement.cpp \
    clientcamera.cpp \
    clientsend.cpp \
    robotinfo.cpp \
    clientreceive.cpp \
    clickablelabel.cpp

HEADERS  += interface.h \
    clientcontrol.h \
    config.h \
    cameratraitement.h \
    clientcamera.h \
    clientsend.h \
    robotinfo.h \
    clientreceive.h \
    clickablelabel.h

FORMS    += interface.ui \
    config.ui

RESOURCES += \
    remotebot.qrc
