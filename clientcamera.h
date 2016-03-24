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

struct Poscam
{
    int x;
    int y;
};

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
        static const int camMaxUp;
        static const int camMaxDown;
        static const int camMaxLeft;
        static const int camMaxRight;
        static const int  setVitesseEnable;
        static const int  setVitesseDisable;
        QString left;
        QString right;
        QString down;
        QString up;
        QString reset;
        QString url;
        Poscam* position;
        Interface* MainInter;
        void initCam();
        void setVitesseVar();
        static ClientCamera* m_instance;
        ClientCamera(Interface *inter);
        ~ClientCamera();
        void urlAccess(QString url);

    private slots:
        void disconnected();

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
