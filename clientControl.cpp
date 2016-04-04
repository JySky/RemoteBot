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
    connectedState=false;
    QObject::connect(&soc, SIGNAL(disconnected()),this, SLOT(disconnect()));
    QObject::connect(&soc, SIGNAL(disconnected()),MainInter, SLOT(robotDisconnected()));
    QObject::connect(this, SIGNAL(connected()),MainInter, SLOT(robotConnected()));
}

void ClientControl::processing()
{
    timer.setInterval(50);
    QObject::connect(&timer, SIGNAL (timeout()), this, SLOT (dataWrite()));
    QObject::connect(&timer, SIGNAL (timeout()), this, SLOT (dataRead()));
    timer.start();
}

void ClientControl::stopProcessing()
{
    timer.stop();
}


void ClientControl::connect()
{
    soc.connectToHost(IP,port);

    if(!soc.waitForConnected(5000))
    {
        connectedState=false;
    }
    else
    {
        connectedState=true;
        soc.open(QIODevice::ReadWrite);
        emit connected();
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
}

void ClientControl::disconnect()
{
    soc.close();
    connectedState=false;
    stopProcessing();
    if(!connectedState)//(soc.state()==QAbstractSocket::UnconnectedState) && soc.isValid())
    {
        std::string s ="Disconnected from"+IP.toStdString()+"/"+(QString::number(port)).toStdString();
        const char* msg=s.c_str();
        QMessageBox::information(
              MainInter,
              tr("Robot"),
              tr(msg) );
        emit disconnected();
    }
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

    long odoL,odoR;
    int speedL,speedR;
    quint8 battery;
    int ir1,ir2,ir3,ir4,current,version;

    // Left
    speedL = (int)((data.at(1) << 8) + data.at(0));
    if (speedL > 32767)
        speedL=speedL-65536;
    //speedL=speedL;

    qDebug()<<"speedL";
    qDebug()<<(int)speedL;


    /**/
    battery = data.at(2);
    /**/


    ir4 = data.at(3);
    ir3 = data.at(4);

    //odoL = data.at(5)+(data.at(6)<<8)+(data.at(7)<<16)+(data.at(8)<<24);
    odoL=((((long)data.at(8) << 24))+(((long)data.at(7) << 16))+(((long)data.at(6) << 8))+((long)data.at(5)));

    // Right
    speedR=(int)((data.at(10) << 8) + data.at(9));
    if (speedR> 32767)
        speedR=speedR-65536;
    speedR=speedR;


    ir1 = data.at(11);
    ir2 = data.at(12);

    odoR=((((long) data.at(16) << 24))+(((long) data.at(15) << 16))+(((long) data.at(14) << 8))+((long) data.at(13)));

    //odoR = data.at(13)+(data.at(14)<<8)+(data.at(15)<<16)+(data.at(16)<<24);

    current = data.at(17);
    version = data.at(18);


    MainInter->setBatLevel((int)battery);
    MainInter->setVitLeft(speedL);
    MainInter->setVitRight(speedR);
    MainInter->setCurrent(current);
    MainInter->setIR1(ir1);
    MainInter->setIR2(ir2);
    MainInter->setIR3(ir3);
    MainInter->setIR4(ir4);
    MainInter->setVersion(version);

}

//************** SLOTS **************//

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
//************** SET GET **************//

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

ClientControl::~ClientControl()
{

}

