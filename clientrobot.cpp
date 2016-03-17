#include "clientrobot.h"
ClientRobot* ClientRobot::m_instance = NULL;

ClientRobot::ClientRobot(QTcpSocket *soc)
{
    this->soc=soc;
    clientCont = ClientControl::getInstance();
    this->start();
}

void ClientRobot::run()
{
    QByteArray data;
    while(1)
    {
        QThread::msleep(25);
        send();
        if(soc->bytesAvailable()>20)
        {
             data=soc->read(21);
             receive(data);
        }
    }


}


void ClientRobot::send()
{
    soc->write(control());
    soc->flush();
}

QByteArray ClientRobot::control()
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
    char * trame=toSend.data();
    quint16 crc=Crc16((unsigned char*)trame,1);
    toSend.append((unsigned char)crc);
    toSend.append((unsigned char)(crc>>8));
    /*toSend.append((unsigned char)1);
    toSend.append((unsigned char)1);*/
    return toSend;
}

short ClientRobot::Crc16(unsigned char *Adresse_tab , unsigned char Taille_max)
{
    unsigned int Crc = 0xFFFF;
    unsigned int Polynome = 0xA001;
    unsigned int CptOctet = 0;
    unsigned int CptBit = 0;
    unsigned int Parity= 0;
    Crc = 0xFFFF;
    Polynome = 0xA001;
    for ( CptOctet= 0 ; CptOctet < Taille_max ; CptOctet++)
    {
        Crc ^= *( Adresse_tab + CptOctet);
        for ( CptBit = 0; CptBit <= 7 ; CptBit++)
        {
            Parity= Crc;
            Crc >>= 1;
            if (Parity%2 == true)
                Crc ^= Polynome;
        }
    }
    return(Crc);
}

void ClientRobot::receive(QByteArray data)
{
    RobotInfo dataL;
    RobotInfo dataR;
    dataL.setSpeedFront((int)((data.at(1) << 8) + data.at(0)));
    if (dataL.getSpeedFront() > 32767)
        dataL.setSpeedFront(dataL.getSpeedFront()-65536);
    dataL.setBatLevel(data.at(2));
    dataL.setIR(data.at(3));
    dataL.setIR2(data.at(4));
    dataL.setodometry(((((long)data.at(8) << 24))+(((long)data.at(7) <<16))+(((long)data.at(6) << 8))+((long)data.at(5))));
    dataR.setSpeedFront(((int)(data.at(10) << 8) + data.at(9)));
    if (dataR.getSpeedFront() > 32767)
        dataR.setSpeedFront(dataR.getSpeedFront()-65536);
    dataR.setBatLevel(0);
    dataR.setIR(data.at(11));
    dataR.setIR2(data.at(12));
    dataR.setodometry(((((long)data.at(16)<< 24))+(((long)data.at(15) <<16))+(((long)data.at(14) << 8))+((long)data.at(13))));
    dataL.setCurrent(data.at(17));
    dataR.setCurrent(data.at(17));
    dataL.setVersion(data.at(18));
    dataR.setVersion(data.at(18));
    MainInter->majInterface(dataR,dataL);
}

ClientRobot::~ClientRobot()
{
    this->exit();
    delete this;
}
