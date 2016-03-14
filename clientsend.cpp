#include "clientsend.h"
ClientSend* ClientSend::m_instance = NULL;

ClientSend::ClientSend(QTcpSocket *soc)
{
    this->soc=soc;
    clientCont = ClientControl::getInstance();
    this->start();
}

void ClientSend::run()
{
    while(1)
    {
        QThread::msleep(25);
        send();
    }
}

void ClientSend::send()
{
    soc->write(control());
    soc->waitForBytesWritten(1000);
}

QByteArray ClientSend::control()
{
    QByteArray toSend;
    unsigned char char7=0;
    toSend.append((unsigned char)255);//char1
    toSend.append((unsigned char)7);//char2
    toSend.append(clientCont->getLeftSpeed());//char3
    toSend.append(clientCont->getLeftSpeed());//char4
    toSend.append(clientCont->getRightSpeed());//char5
    toSend.append(clientCont->getRightSpeed());//char6
    //char7 build
    char7+=128*clientCont->getLeftSpeedLoop();
    char7+=64*clientCont->getLeftSpeedFlag();
    char7+=32*clientCont->getRightSpeedLoop();
    char7+=16*clientCont->getRightSpeedFlag();
    char7+=8*0;
    toSend.append(char7);//char7
    return toSend;
}
ClientSend::~ClientSend()
{
    this->exit();
    delete this;
}
