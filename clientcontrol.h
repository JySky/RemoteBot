#ifndef CLIENTCONTROL_H
#define CLIENTCONTROL_H
#include <QtNetwork>
#include <iostream>

class ClientControl
{
    public:
        static ClientControl* getInstance(int p,QString IP){
            if ( m_instance == NULL ) {
                    m_instance = new ClientControl(p,IP);
                  }
            return m_instance;
        }
        /*QTcpSocket getSocket(){
            return soc;
        }*/
        void receive();
        void send();

    private:
        QString IP;
        int port;
        QTcpSocket soc;
        unsigned char rightSpeed;
        unsigned char leftSpeed;
        unsigned char leftSpeedLoop;
        unsigned char rightSpeedLoop;
        static ClientControl* m_instance;
        QByteArray control();

        ClientControl(int p,QString IP);
        ~ClientControl();
};

#endif // CLIENTCONTROL_H
