#ifndef CLIENTRECEIVETRAITEMENT_H
#define CLIENTRECEIVETRAITEMENT_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include "robotinfo.h"


class ClientReceiveTraitement : public QThread
{
        Q_OBJECT
    private:
        QTcpSocket* soc;
        static ClientReceiveTraitement* m_instance;
        ClientReceiveTraitement(QTcpSocket* soc);
        ~ClientReceiveTraitement();
        void receive();
    protected:
        void run();
    public :
        static ClientReceiveTraitement* getInstance(QTcpSocket* soc)
        {
            if ( m_instance == NULL )
            {
                m_instance = new ClientReceiveTraitement(soc);
            }
            return m_instance;
        }
};

#endif // CLIENTRECEIVETRAITEMENT_H
