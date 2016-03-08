#ifndef CLIENTRECEIVETRAITEMENT_H
#define CLIENTRECEIVETRAITEMENT_H

#include <QObject>
#include <QThread>

class ClientReceiveTraitement : public QThread
{
        Q_OBJECT
    public:
        ClientReceiveTraitement();

    protected:
        void run();
};

#endif // CLIENTRECEIVETRAITEMENT_H
