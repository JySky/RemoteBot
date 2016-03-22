#ifndef CLIENTCAMERA_H
#define CLIENTCAMERA_H
#include <QtNetwork>
#include <iostream>
#include <QMessageBox>
#include "interface.h"
#include <QMainWindow>
#include <QObject>
#include <QMessageBox>
#include <QInputDialog>
#include <QIcon>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>

class Interface;

class ClientCamera: public QObject
{
    Q_OBJECT

    private:
        QString IP;
        QTcpSocket soc;
        int port;
        bool connected;
        bool camAuto;
        static const int camV;
        QString left;
        QString right;
        QString down;
        QString up;
        QString reset;
        QString url;
        QNetworkAccessManager* mgr;
        QNetworkRequest* req;
        Interface* MainInter;
        void initCam();
        void setVitesseVar();
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
        bool connecttoCamera();
        void stopConnectionCamera();
        void setCamAuto(bool c){camAuto=c;}
        void moveCam(int pos);
};

#endif // CLIENTCAMERA_H
