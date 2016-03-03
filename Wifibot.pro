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
    config.cpp

HEADERS  += interface.h \
    clientcontrol.h \
    config.h

FORMS    += interface.ui \
    config.ui
INCLUDEPATH += C:\opencv\sources\include
LIBS += "C:\opencv\build\include\*.dll"
