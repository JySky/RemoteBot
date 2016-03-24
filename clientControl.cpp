#include "clientcontrol.h"
#include <QDebug>

ClientControl* ClientControl::m_instance = NULL;

using namespace std;

ClientControl::ClientControl(Interface *inter)
{
    MainInter=inter;
    port=15020;
    IP="192.168.1.106";
    rightSpeed=0;
    leftSpeed=0;
    rightSpeedFlag=0;
    leftSpeedFlag=0;
    leftSpeedLoop=0;
    rightSpeedLoop=0;
    connected=false;
    connect(&soc, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(&soc, SIGNAL(disconnected()),MainInter, SLOT(robotDisconnected()));
}

void ClientControl::processing()
{
    timer.setInterval(50);
    timer2.setInterval(100);
    connect(&timer, SIGNAL (timeout()), this, SLOT (dataWrite()));
    connect(&timer2, SIGNAL (timeout()), this, SLOT (dataRead()));
    timer.start();
    timer2.start();
}

void ClientControl::stopProcessing()
{
    timer.stop();
    timer2.stop();
}

void ClientControl::dataWrite()
{
    if(soc.open(QIODevice::ReadWrite))
    {
        send();
    }
}
void ClientControl::dataRead()
{
    if(soc.open(QIODevice::ReadWrite))
    {
        QByteArray data;
        soc.waitForReadyRead(100);
        if(soc.bytesAvailable()>=21)
        {
            data=soc.read(21);
            receive(data);
        }
    }
}

bool ClientControl::connecttoRobot()
{
    soc.connectToHost(IP,port);

    if(!soc.waitForConnected(5000))
    {
        connected=false;
    }
    else
    {
        connected=true;
        soc.open(QIODevice::ReadWrite);
        processing();
    }
    if(soc.state()==QAbstractSocket::UnconnectedState)
    {
        std::string s ="Not connected to"+IP.toStdString()+"/"+(QString::number(port)).toStdString();
        const char* msg=s.c_str();
        QMessageBox::critical(
              MainInter,
              tr("Robot"),
              tr(msg) );
    }
    return connected;
}

void ClientControl::setRightSpeed(unsigned char speed)
{
    rightSpeed = speed;
}

void ClientControl::setLeftSpeed(unsigned char speed)
{
    leftSpeed = speed;
}

void ClientControl::setRightSpeedFlag(unsigned char flag)
{
    rightSpeedFlag = flag;
}

void ClientControl::setLeftSpeedFlag(unsigned char flag)
{
    leftSpeedFlag = flag;
}
void ClientControl::setIp(QString i)
{
    IP = i;
}

void ClientControl::setPort(int p)
{
    port = p;
}

bool ClientControl::stopConnectionRobot()
{
    soc.close();
    connected=false;
    stopProcessing();
    if((soc.state()==QAbstractSocket::UnconnectedState) && soc.isValid())
    {
        std::string s ="Disconnected from"+IP.toStdString()+"/"+(QString::number(port)).toStdString();
        const char* msg=s.c_str();
        QMessageBox::information(
              MainInter,
              tr("Robot"),
              tr(msg) );
    }
    MainInter->setcolorConnected("red");
    return connected;

}

void ClientControl::disconnected()
{
    soc.close();
    connected=false;
    timer.stop();
    timer2.stop();
    if(soc.state()==QAbstractSocket::UnconnectedState)
    {
        std::string s ="Disconnected from"+IP.toStdString()+"/"+(QString::number(port)).toStdString();
        const char* msg=s.c_str();
        QMessageBox::information(
              MainInter,
              tr("Robot"),
              tr(msg) );
    }
    MainInter->setcolorConnected("red");
}

void ClientControl::send()
{
    soc.waitForBytesWritten(10);
    soc.write(control());
    soc.flush();
}

QByteArray ClientControl::control()
{
    QByteArray toSend;
    int char7=0;
    toSend.clear();
    toSend.clear();
    toSend.append((char)0xff);
    toSend.append((char)0x07);
    toSend.append((char)getLeftSpeed());
    toSend.append((char)0);
    toSend.append((char)getRightSpeed());
    toSend.append((char)0);
    char7+=128*0;//getLeftSpeedLoop();
    char7+=64*getLeftSpeedFlag();
    char7+=32*0;//getRightSpeedLoop();
    char7+=16*getRightSpeedFlag();
    char7+=8*0;
    toSend.append((char)char7);

    quint16 crc = Crc16( &toSend, 1);
    toSend.append((char)crc);
    toSend.append((char)(crc>>8));
    return toSend;
}

quint16 ClientControl::Crc16(QByteArray* byteArray, int pos){
    unsigned char *data = (unsigned char* )byteArray->constData();
    quint16 crc = 0xFFFF;
    quint16 Polynome = 0xA001;
    quint16 Parity = 0;
    for(; pos < byteArray->length(); pos++){
        crc ^= *(data+pos);
        for (unsigned int CptBit = 0; CptBit <= 7 ; CptBit++){
            Parity= crc;
            crc >>= 1;
            if (Parity%2 == true) crc ^= Polynome;
        }
    }
    return crc;
}

void ClientControl::receive(QByteArray data)
{
    RobotInfo dataL;
    RobotInfo dataR;
    dataL.setSpeedFront((int)((data.at(1) << 8) + data.at(0)));
    if (dataL.getSpeedFront() > 32767)
        dataL.setSpeedFront(dataL.getSpeedFront()-65536);
    dataL.setBatLevel(data.at(2));
    qDebug()<<((int)data.at(2));
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
    qDebug()<<data;
    MainInter->majInterface(dataR,dataL);
}

ClientControl::~ClientControl()
{

}

