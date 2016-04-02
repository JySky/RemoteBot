#ifndef INTERFACE_H
#define INTERFACE_H
#include "clientcontrol.h"
#include "config.h"
#include <QMainWindow>
#include <iostream>
#include <stdio.h>
#include "clientcamera.h"
#include "clickablelabel.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QIcon>
#include <QString>
#include <iostream>
#include <about.h>
#include <QThread>

namespace Ui {
    class Interface;
}

class ClientControl;
class ClientCamera;
class About;

class Interface : public QMainWindow
{
        Q_OBJECT

        private:
            QThread controlThread;
            Ui::Interface *ui;
            ClientControl *Clientcont;
            ClientCamera *Clientcam;
            bool Ipressed;
            bool Jpressed;
            bool Kpressed;
            bool Lpressed;
            bool Zpressed;
            bool Qpressed;
            bool Spressed;
            bool Dpressed;
            bool camAuto;
            bool imgProc;
            bool controllerOn;
            bool camconnected;
            bool robotconnected;
            void ControlDirection();
            void ControlCam();
            void majConnectedState();

        protected:
            void keyPressEvent(QKeyEvent *event);
            void keyReleaseEvent(QKeyEvent *event);

        private slots:
            void on_robotStart_clicked();
            void on_actionPort_et_IP_triggered();
            void on_robotStop_clicked();
            void on_cameraLeft_clicked();
            void on_cameraLeft_released();
            void on_cameraUp_released();
            void on_cameraUp_clicked();
            void on_cameraDown_released();
            void on_cameraDown_clicked();
            void on_cameraRight_released();
            void on_cameraRight_clicked();
            void on_dotCamera_clicked();
            void on_dotCamera_released();
            void on_robotRight_clicked();
            void on_robotRight_released();
            void on_robotUp_clicked();
            void on_robotUp_released();
            void on_robotDown_clicked();
            void on_robotDown_released();
            void on_robotLeft_clicked();
            void on_robotLeft_released();
            void on_actionQuitter_triggered();
            void on_actionActiver_Manette_changed();
            void on_actionActiver_Traitement_Image_changed();
            void on_actionCamera_Automatique_changed();
            void on_actionA_propos_triggered();

        public:
            Ui::Interface *getUi(){return ui;}
            explicit Interface(QWidget *parent = 0);
            ~Interface();
            void setcolorConnected(QString color);
            void setQWebView(QString link);
            void setImage(QImage img);
            void setImage(QString img);
            int getSliderCam();
            void setBatLevel(int lvl);
            void setVitLeft(int lvl);
            void setVitRight(int lvl);

        public slots :
            void camDisconnected();
            void camStreamState();
            void camConnected();
            void robotDisconnected();
            void robotConnected();

        signals:
            void robotConnect();
            void robotDisconnect();
            void camConnect();
            void camDisconnect();

};

#endif // INTERFACE_H
