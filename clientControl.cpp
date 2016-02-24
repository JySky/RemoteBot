#include "clientcontrol.h"

using namespace std;



ClientControl::ClientControl(int p,QString IP)
{

    port=p; // choix arbitraire (>1024)
    this->IP=IP;
   /* QObject::connect(&soc,SIGNAL(connected()),this,SLOT(connexion_OK()));
    // signal émis lors de la connexion au serveur
    QObject:: connect(&soc, SIGNAL(readyRead()), this, SLOT(lecture()));
    // signal émis lorsque des données sont prêtes à être lues*/
    soc.connectToHost(IP,port);
    if(!soc.waitForConnected(5000))
    {
        cout<<soc.error();
    }
    rightSpeed=0;
    leftSpeed=0;
}

void ClientControl::receive()
{
    QByteArray buffer;
    buffer=soc.readAll();
}

void ClientControl::send()
{
    soc.write(control());
}

QByteArray control()
{
    QByteArray toSend;
    char char7[8];
    toSend.append((unsigned char)255);
    toSend.append((unsigned char)7);
    toSend.append((unsigned char)leftSpeed);
    toSend.append((unsigned char)leftSpeed);
    toSend.append((unsigned char)rightSpeed);
    toSend.append((unsigned char)rightSpeed);


    return toSend;
}

ClientControl::~ClientControl()
{

}

