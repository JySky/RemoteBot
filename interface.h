#ifndef INTERFACE_H
#define INTERFACE_H
#include "simplexbox360controller.h"
#include "clientcontrol.h"
#include "config.h"
#include <QMainWindow>
#include <iostream>
#include <stdio.h>
#include "clickablelabel.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QIcon>
#include <QString>
#include <iostream>
#include <about.h>
#include <QThread>
#include <QMutex>
#include <QtMath>


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
            QMutex mutex;
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
            bool bImshow;
            bool controllerOn;
            bool camconnected;
            bool robotconnected;
            int speedL;
            int speedR;
            int IR1;
            int oldIR1;
            int IR2;
            int oldIR2;
            int IR3;
            int oldIR3;
            int IR4;
            int oldIR4;
            long odoL;
            long odoR;
            long toltalDistance;
            void setVitesseGlobal();
            void ControlDirection();
            void ControlCam();
            void majConnectedState();
            void initinterface();
            void initConnect();
            void computeDistance();
            SimpleXbox360Controller* controller1;
            SimpleXbox360Controller::InputState currentGamepadState;
            bool gamepadConnected;
            void controller();

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
            void on_actionImshow_OpenCV_changed();
            void on_actionAbout_triggered();

        public:
            Ui::Interface *getUi(){return ui;}
            explicit Interface(QWidget *parent = 0);
            ~Interface();
            void setcolorConnected(QString color);
            void setImage(QImage img);
            void setImage(QString img);

        public slots :
            //void controllerConnected(uint controllerNum);
            void displayGamepadState(SimpleXbox360Controller::InputState GamepadState);
            void GamepadConnected(void);
            void GamepadDisconnected(void);

            void setBatLevel(int lvl);
            void setVitLeft(int lvl);
            void setVitRight(int lvl);
            void setIR1(int lvl);
            void setIR2(int lvl);
            void setIR3(int lvl);
            void setIR4(int lvl);
            void setVersion(int vers);
            void setCurrent(int curr);
            void setOdoL(long odo);
            void setOdoR(long odo);
            void camDisconnected();
            void camNotConnected();
            void camConnected();
            void robotDisconnected();
            void robotConnected();
            void robotNotConnected();
            void setFrame(QImage fr);
            void getSliderCamValue();
            void receiveIPCam(QString ipc);
            void receivePortCam(int portc);
            void receiveIPRobot(QString ipr);
            void receivePortRobot(int portr);

        signals:
            void startImgProcessing();
            void stopImgProcessing();
            void robotConnect();
            void robotDisconnect();
            void camConnect();
            void camDisconnect();
            void startImshow();
            void stopImshow();
            void RightSpeed(unsigned char);
            void LeftSpeed(unsigned char);
            void RightSpeedFlag(unsigned char);
            void LeftSpeedFlag(unsigned char);
            void sendSliderCamValue(int);

            void getIPCam();
            void getPortCam();
            void getIPRobot();
            void getPortRobot();
            void setIPCam(QString);
            void setPortCam(int);
            void setIPRobot(QString);
            void setPortRobot(int);
            void setConfigIPCam(QString);
            void setConfigPortCam(int);
            void setConfigIPRobot(QString);
            void setConfigPortRobot(int);

            void robotFrontCollisionOn();
            void robotFrontCollisionOff();
            void robotRearCollisionOn();
            void robotRearCollisionOff();

            void moveCamera(int);
            void vibration(float,float);
};

#endif // INTERFACE_H
