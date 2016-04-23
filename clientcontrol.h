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
#include <QThread>
#include <QMutex>

class Interface;

using namespace std;

class ClientControl : public QThread
{
    Q_OBJECT

    private:

        long odoL,odoR;
        int speedL,speedR;
        quint8 battery;
        int ir1,ir2,ir3,ir4,current,version;
        ClientControl(QObject *parent,Interface *inter);
        QMutex mutex;
        static QString IP;
        static int port;
        QTcpSocket *soc;
        QTimer timer,timer2;
        bool connectedState;
        bool frontCollision;
        bool rearCollision;
        unsigned char rightSpeed;
        unsigned char leftSpeed;
        unsigned char leftSpeedLoop;
        unsigned char rightSpeedLoop;
        unsigned char rightSpeedFlag;
        unsigned char leftSpeedFlag;
        Interface* MainInter;
        static ClientControl* m_instance;
        QByteArray control();
        QByteArray control(int leftspeed,int rightspeed, int leftflag, int rightflag);
        void receive(QByteArray data);
        quint16 Crc16(QByteArray* byteArray, int pos);
        void send();
        void receive();
        void init();
        bool STOP;
        void dataWrite();
        void dataRead();
        void connect();
        void disconnect();
        ~ClientControl();

    public slots:
        void stop();
        void robotStart();
        void socketDisconnected();
        void setIP(QString IP);
        void setPort(int Port);
        void getIP();
        void getPort();
        void setRightSpeed(unsigned char speed);
        void setLeftSpeed(unsigned char speed);
        void setRightSpeedFlag(unsigned char flag);
        void setLeftSpeedFlag(unsigned char flag);

        void frontCollisionOn();
        void frontCollisionOff();
        void rearCollisionOn();
        void rearCollisionOff();

    signals:
        void connected();
        void disconnected();
        void socNotConnected();

        void BatLevel(int);
        void VitLeft(int);
        void VitRight(int);
        void IR1(int);
        void IR2(int);
        void IR3(int);
        void IR4(int);
        void Version(int);
        void Current(int);
        void ODOL(long);
        void ODOR(long);
        void sendIP(QString);
        void sendPort(int);



    public:
        static ClientControl* getInstance(){return m_instance;}
        static ClientControl* getInstance(QObject* parent,Interface* inter)
        {
            if ( m_instance == NULL )
            {
                m_instance = new ClientControl(parent,inter);
            }
            return m_instance;
        }
        void run();

};

#endif // CLIENTCONTROL_H
