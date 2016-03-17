#ifndef CAMERATRAITEMENT_H
#define CAMERATRAITEMENT_H

#include <QObject>
#include <QThread>
#include <stdio.h>
#include <iostream>


class CameraTraitement : public QThread
{
        Q_OBJECT
    public:
        CameraTraitement();

    protected:
        void run();
};

#endif // CAMERATRAITEMENT_H
