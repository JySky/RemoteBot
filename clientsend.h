#ifndef CLIENTSEND_H
#define CLIENTSEND_H
#include <QTcpSocket>
#include <QObject>
#include <QThread>
#include <QTimer>
#include "clientcontrol.h"

class ClientControl;
class ClientSend : public QThread
{
        Q_OBJECT
    private:
        QTcpSocket* soc;
        QTimer* thr;
        static ClientSend* m_instance;
        ClientControl* clientCont;
        ClientSend(QTcpSocket *soc);
        ~ClientSend();
        QByteArray control();
    protected:
        void run();
    public:
        static ClientSend* getInstance(QTcpSocket* soc)
        {
            if ( m_instance == NULL )
            {
                m_instance = new ClientSend(soc);
            }
            return m_instance;
        }

        void send();
};

#endif // CLIENTSEND_H
