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
win32:LIBS += $${_PRO_FILE_PWD_}/XInput.lib

OTHER_FILES += \
    XInput.lib

win32: RC_ICONS = RemoteBot.ico
SOURCES += main.cpp\
        interface.cpp \
    clientcontrol.cpp \
    config.cpp \
    clientcamera.cpp \
    clickablelabel.cpp \
    about.cpp \
    simplexbox360controller.cpp

HEADERS  += interface.h \
    clientcontrol.h \
    config.h \
    clientcamera.h \
    clickablelabel.h \
    about.h \
    XInput.h \
    simplexbox360controller.h

FORMS    += interface.ui \
    config.ui \
    about.ui

RESOURCES += \
    remotebot.qrc

INCLUDEPATH += C:/OpencvQT/install/include \
            C:/OpencvQT/install/highgui

LIBS += -L C:\\OpencvQT\install\x86\mingw\bin \
    libopencv_core310 \
    libopencv_highgui310 \
    libopencv_imgproc310 \
    libopencv_features2d310 \
    libopencv_calib3d310 \
    libopencv_videoio310

LIBS += -LC:\\OpencvQT\install\x86\mingw\lib \
    libopencv_highgui310 \
    libopencv_core310   \
    libopencv_video310     \
    libopencv_videostab310 \
    libopencv_imgproc310 \
    libopencv_features2d310 \
    libopencv_calib3d310 \
    libopencv_videoio310

LIBS += -LC:\\OpencvQT\bin \
    libopencv_core310 \
    libopencv_highgui310 \
    libopencv_imgproc310 \
    libopencv_features2d310 \
    libopencv_calib3d310 \
    libopencv_videoio310

LIBS += -LC:\\OpencvQT\lib \
    libopencv_highgui310 \
    libopencv_core310   \
    libopencv_video310     \
    libopencv_videostab310 \
    libopencv_imgproc310 \
    libopencv_features2d310 \
    libopencv_calib3d310 \
    libopencv_videoio310
