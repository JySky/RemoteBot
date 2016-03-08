#-------------------------------------------------
#
# Project created by QtCreator 2016-02-24T21:12:43
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Wifibot
TEMPLATE = app


SOURCES += main.cpp\
        interface.cpp \
    clientcontrol.cpp \
    config.cpp \
    cameratraitement.cpp \
    clientreceivetraitement.cpp

HEADERS  += interface.h \
    clientcontrol.h \
    config.h \
    cameratraitement.h \
    clientreceivetraitement.h

FORMS    += interface.ui \
    config.ui

INCLUDEPATH += C:/OpencvQT/install/include

LIBS += -LC:\\OpencvQT\\bin \
    libopencv_core310 \
    libopencv_highgui310 \
    libopencv_imgproc310 \
    libopencv_features2d310 \
    libopencv_calib3d310 \
