#ifndef INTERFACE_H
#define INTERFACE_H
#include "clientcontrol.h"
#include "config.h"
#include <QMainWindow>
#include <iostream>
#include <stdio.h>
#include "clientcamera.h"

namespace Ui {
class Interface;
}
class ClientControl;
class ClientCamera;

class Interface : public QMainWindow
{
    Q_OBJECT

    private:
        QNetworkAccessManager mgr;
        QNetworkRequest req;
        Ui::Interface *ui;
        ClientControl *Clientcont;
        ClientCamera *Clientcam;
    protected:
        void keyPressEvent(QKeyEvent *event);
        void keyReleaseEvent(QKeyEvent *event);

    private slots:
        void on_robotStart_clicked();
        void on_actionPort_et_IP_triggered();

        void on_robotStop_clicked();

public:
        Ui::Interface *getUi(){return ui;}
        explicit Interface(QWidget *parent = 0);
        ~Interface();
        void setcolorConnected(QString color);
};

#endif // INTERFACE_H
