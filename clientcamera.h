#ifndef CLIENTCAMERA_H
#define CLIENTCAMERA_H
#include <QtNetwork>
#include <iostream>
#include <QMessageBox>
#include "interface.h"
#include <QMainWindow>

class Interface;

class ClientCamera
{
    private:
        QString IP;
        int port;
        QTcpSocket soc;
        Interface* MainInter;
        static ClientCamera* m_instance;
        ClientCamera(Interface *inter);
        ~ClientCamera();

    public:
        static ClientCamera* getInstance(Interface* inter)
        {
            if ( m_instance == NULL )
            {
                m_instance = new ClientCamera(inter);
            }
            return m_instance;
        }
        void setIp(QString i);
        void setPort(int p);
        QString getIp(){return IP;}
        int getPort(){return port;}
        void connecttoCamera();
};

#endif // CLIENTCAMERA_H
