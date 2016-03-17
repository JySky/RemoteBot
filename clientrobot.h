#ifndef CLIENTROBOT_H
#define CLIENTROBOT_H
#include <QTcpSocket>
#include <QObject>
#include <QThread>
#include <QTimer>
#include "clientcontrol.h"

class ClientControl;
class ClientRobot : public QObject
{
        Q_OBJECT
    private:
        QTcpSocket* soc;
        QTimer* thr;
        static ClientRobot* m_instance;
        ClientControl* clientCont;
        ClientRobot(QTcpSocket *soc);
        ~ClientRobot();
        QByteArray control();
        void receive(QByteArray data);
        short Crc16(unsigned char *Adresse_tab , unsigned char Taille_max);
        void send();

    protected:
        void run();
    public:
        static ClientRobot* getInstance(QTcpSocket* soc)
        {
            if ( m_instance == NULL )
            {
                m_instance = new ClientRobot(soc);
            }
            return m_instance;
        }


};

#endif // CLIENTROBOT_H
