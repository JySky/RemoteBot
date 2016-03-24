#ifndef CLIENTCONTROL_H
#define CLIENTCONTROL_H
#include <QtNetwork>
#include <iostream>
#include <QMessageBox>
#include "interface.h"
#include <QMainWindow>
#include <QTimer>
#include <QObject>
#include <QMessageBox>
#include <QInputDialog>
#include <QIcon>
#include <QString>

class Interface;

class ClientControl: public QObject
{
    Q_OBJECT

    private:
        QString IP;
        int port;
        QTcpSocket soc;
        QTimer timer,timer2;
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
        QByteArray control();
        void receive(QByteArray data);
        quint16 Crc16(QByteArray* byteArray, int pos);
        void send();
        void processing();
        void stopProcessing();
        ~ClientControl();

    private slots:
        void dataWrite();
        void dataRead();
        void disconnected();

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
        bool connecttoRobot();
        bool stopConnectionRobot();
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
        QTcpSocket* getSocket(){return &soc;}
};

#endif // CLIENTCONTROL_H
