#ifndef CLIENTCONTROL_H
#define CLIENTCONTROL_H
#include <QtNetwork>
#include <iostream>
#include <QMessageBox>
#include "interface.h"
#include <QMainWindow>
#include "clientsend.h"
#include "clientreceivetraitement.h"

class Interface;
class ClientSend;
class ClientReceiveTraitement;

class ClientControl
{
    private:
        QString IP;
        int port;
        QTcpSocket soc;
        bool connected;
        unsigned char rightSpeed;
        unsigned char leftSpeed;
        unsigned char leftSpeedLoop;
        unsigned char rightSpeedLoop;
        unsigned char rightSpeedFlag;
        unsigned char leftSpeedFlag;
        Interface* MainInter;
        static ClientControl* m_instance;
        ClientControl(Interface *inter);
        ~ClientControl();

    public:
        static ClientControl* getInstance(){return m_instance;}
        static ClientControl* getInstance(Interface* inter)
        {
            if ( m_instance == NULL )
            {
                m_instance = new ClientControl(inter);
            }
            return m_instance;
        }
        void receive();
        void init();
        void connecttoRobot();
        void stopConnectionRobot();
        void setRightSpeed(unsigned char speed);
        void setLeftSpeed(unsigned char speed);
        void setRightSpeedFlag(unsigned char flag);
        void setLeftSpeedFlag(unsigned char flag);
        void setIp(QString i);
        void setPort(int p);
        unsigned char getLeftSpeedFlag(){return leftSpeedFlag;}
        unsigned char getRightSpeedFlag(){return rightSpeedFlag;}
        unsigned char getLeftSpeedLoop(){return leftSpeedLoop;}
        unsigned char getRightSpeedLoop(){return rightSpeedLoop;}
        unsigned char getRightSpeed(){return rightSpeed;}
        unsigned char getLeftSpeed(){return leftSpeed;}
        unsigned char getConnected(){return connected;}
        QString getIp(){return IP;}
        int getPort(){return port;}
};

#endif // CLIENTCONTROL_H
