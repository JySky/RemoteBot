#ifndef CLIENTRECEIVE_H
#define CLIENTRECEIVE_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include "robotinfo.h"
#include "interface.h"

class Interface;

class ClientReceive : public QThread
{
        Q_OBJECT
    private:
        QTcpSocket* soc;
        static ClientReceive* m_instance;
        ClientReceive(QTcpSocket* soc,Interface *inter);
        ~ClientReceive();
        void receive(QByteArray data);
        bool stopR;
        Interface* MainInter;
    protected:
        void run();
    public :
        static ClientReceive* getInstance(QTcpSocket* soc,Interface *inter)
        {
            if ( m_instance == NULL )
            {
                m_instance = new ClientReceive(soc,inter);
            }
            return m_instance;
        }
        void stop(){stopR=false;}
};

#endif // CLIENTRECEIVE_H
