#include "clientreceivetraitement.h"

ClientReceiveTraitement* ClientReceiveTraitement::m_instance = NULL;

ClientReceiveTraitement::ClientReceiveTraitement(QTcpSocket *soc)
{
    this->soc=soc;
    this->start();
}

void ClientReceiveTraitement::run()
{

    while(1)
    {
        QThread::msleep(25);
        receive();
    }
}
void ClientReceiveTraitement::receive()
{
    QByteArray data;
    data=soc->read(21);
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
}

ClientReceiveTraitement::~ClientReceiveTraitement()
{

}
