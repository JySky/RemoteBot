#include "clientcontrol.h"
#include <QDebug>

ClientControl* ClientControl::m_instance = NULL;
int ClientControl::port=15020;
QString ClientControl::IP="192.168.1.106";




//********************************** CONSTRUCTOR **********************************//

ClientControl::ClientControl(QObject *parent, Interface *inter) :
        QThread(parent)
{
    MainInter=inter;
    QObject::connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    QObject::connect(this, SIGNAL(connected()),MainInter, SLOT(robotConnected()), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(disconnected()),MainInter, SLOT(robotDisconnected()), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(socNotConnected()),MainInter, SLOT(robotNotConnected()), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(BatLevel(int)),MainInter, SLOT(setBatLevel(int)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(VitLeft(int)),MainInter, SLOT(setVitLeft(int)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(VitRight(int)),MainInter, SLOT(setVitRight(int)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(IR1(int)),MainInter, SLOT(setIR1(int)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(IR2(int)),MainInter, SLOT(setIR2(int)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(IR3(int)),MainInter, SLOT(setIR3(int)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(IR4(int)),MainInter, SLOT(setIR4(int)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(Version(int)),MainInter, SLOT(setVersion(int)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(Current(int)),MainInter, SLOT(setCurrent(int)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(ODOL(long)),MainInter, SLOT(setOdoL(long)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(ODOR(long)),MainInter, SLOT(setOdoR(long)), Qt::QueuedConnection);
    init();
}

void ClientControl::init()
{
    connectedState=false;
    odoL=0;
    odoR=0;
    speedL=0;
    speedR=0;
    battery=0;
    ir1=0;
    ir2=0;
    ir3=0;
    ir4=0;
    current=0;
    version=0;
    STOP=false;
    rightSpeed=0;
    leftSpeed=0;
    rightSpeedFlag=0;
    leftSpeedFlag=0;
    leftSpeedLoop=0;
    rightSpeedLoop=0;
    connectedState=false;
}

//********************************** THREAD MANAGEMENT **********************************//
void ClientControl::run()
{
    connect();
    STOP=false;
    if(connectedState)
    {
        while(!STOP)
        {
            //mutex.lock();
            dataWrite();
            //mutex.unlock();
            msleep(25);
            //mutex.lock();
            dataRead();
            //mutex.unlock();
            emit BatLevel(battery);
            emit VitLeft(speedL);
            emit VitRight(speedR);
            emit IR1(ir1);
            emit IR2(ir2);
            emit IR3(ir3);
            emit IR4(ir4);
            emit Version(version);
            emit Current(current);
            emit ODOL(odoL);
            emit ODOR(odoR);
        }
    }
    soc->close();
    m_instance=NULL;
}

//********************************** SOCKET MANAGEMENT **********************************//

void ClientControl::dataWrite()
{
    if(soc->open(QIODevice::ReadWrite))
    {
        send();
    }
}

void ClientControl::dataRead()
{
    if(soc->open(QIODevice::ReadWrite))
    {
        QByteArray data;
        soc->waitForReadyRead(100);
        if(soc->bytesAvailable()>=21)
        {
            data=soc->read(21);
            receive(data);
        }
    }
}

void ClientControl::connect()
{
    soc=new QTcpSocket();
    QObject::connect(soc, SIGNAL(disconnected()),this, SLOT(socketDisconnected()), Qt::QueuedConnection);
    QObject::connect(soc, SIGNAL(disconnected()),MainInter, SLOT(robotDisconnected()), Qt::QueuedConnection);
    soc->connectToHost(IP,port);

    if(!soc->waitForConnected(1000))
    {
        STOP=true;
        connectedState=false;
        emit socNotConnected();
    }
    else if(soc->state()==QAbstractSocket::UnconnectedState)
    {
        STOP=true;
        emit socNotConnected();
    }
    else
    {
        connectedState=true;
        soc->open(QIODevice::ReadWrite);
        emit connected();
    }

}

void ClientControl::disconnect()
{
    emit disconnected();
    STOP=true;
    connectedState=false;
}

void ClientControl::send()
{
    soc->waitForBytesWritten(10);
    soc->write(control());
    soc->flush();
}


//********************************** SLOTS **********************************//

void ClientControl::robotStart()
{
    this->start();
}

void ClientControl::stop()
{
    STOP=true;
}

void ClientControl::socketDisconnected()
{
    disconnect();
    STOP=true;
}



//********************************** DATA PROCESSING **********************************//

QByteArray ClientControl::control()
{
    QByteArray toSend;
    int char7=0;
    toSend.clear();
    toSend.append((char)0xff);
    toSend.append((char)0x07);
    if(frontCollision && !rearCollision)
    {
        toSend.append((char)200);
        toSend.append((char)0);
        toSend.append((char)200);
        toSend.append((char)0);
        char7+=128*0;
        char7+=64*0;
        char7+=32*0;
        char7+=16*0;
        char7+=8*0;
    }
    else if(!frontCollision && rearCollision)
    {
        toSend.append((char)200);
        toSend.append((char)0);
        toSend.append((char)200);
        toSend.append((char)0);
        char7+=128*0;
        char7+=64*1;
        char7+=32*0;
        char7+=16*1;
        char7+=8*0;
    }
    else
    {
        toSend.append((char)leftSpeed);
        toSend.append((char)0);
        toSend.append((char)rightSpeed);
        toSend.append((char)0);
        char7+=128*0;
        char7+=64*leftSpeedFlag;
        char7+=32*0;
        char7+=16*rightSpeedFlag;
        char7+=8*0;
    }
    toSend.append((char)char7);
    quint16 crc = Crc16( &toSend, 1);
    toSend.append((char)crc);
    toSend.append((char)(crc>>8));
    return toSend;
}

QByteArray ClientControl::control(int leftspeed,int rightspeed, int leftflag, int rightflag)
{
    QByteArray toSend;
    int char7=0;
    toSend.clear();
    toSend.clear();
    toSend.append((char)0xff);
    toSend.append((char)0x07);
    toSend.append((char)leftspeed);
    toSend.append((char)0);
    toSend.append((char)rightspeed);
    toSend.append((char)0);
    char7+=128*0;
    char7+=64*leftflag;
    char7+=32*0;
    char7+=16*rightflag;
    char7+=8*0;
    toSend.append((char)char7);
    quint16 crc = Crc16( &toSend, 1);
    toSend.append((char)crc);
    toSend.append((char)(crc>>8));
    return toSend;
}

quint16 ClientControl::Crc16(QByteArray* byteArray, int pos)
{
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

    mutex.lock();
        // Left
        speedL = ((((unsigned char)data.at(1) << 8)) + ((unsigned char)data.at(0)));
        if (speedL> 32767)
        {
            speedL-=65536;
        }
        battery = data.at(2);
        ir4 = (unsigned char)data.at(3);
        ir3 = (unsigned char)data.at(4);
        odoL=((((unsigned char)data.at(8) << 24))+(((unsigned char)data.at(7) << 16))+(((unsigned char)data.at(6) << 8))+((unsigned char)data.at(5)));
        // Right
        speedR = ((((unsigned char)data.at(10) << 8)) + ((unsigned char)data.at(9)));
        if (speedR> 32767)
        {
            speedR-=65536;
        }
        qDebug()<<"speed";
        qDebug()<<(speedR+speedL/2);
        ir1 = (unsigned char)data.at(11);
        ir2 = (unsigned char)data.at(12);
        odoR=((((long) data.at(16) << 24))+(((long) data.at(15) << 16))+(((long) data.at(14) << 8))+((long) data.at(13)));
        current = data.at(17);
        version = data.at(18);
    mutex.unlock();
}


//********************************** SET GET **********************************//

void ClientControl::setRightSpeed(unsigned char speed)
{
    mutex.lock();
    rightSpeed = speed;
    mutex.unlock();
}

void ClientControl::setLeftSpeed(unsigned char speed)
{
    mutex.lock();
    leftSpeed = speed;
    mutex.unlock();
}

void ClientControl::setRightSpeedFlag(unsigned char flag)
{
    mutex.lock();
    rightSpeedFlag = flag;
    mutex.unlock();
}

void ClientControl::setLeftSpeedFlag(unsigned char flag)
{
    mutex.lock();
    leftSpeedFlag = flag;
    mutex.unlock();
}

void ClientControl::setIP(QString IP)
{
    if(!connectedState)
    {
        mutex.lock();
        this->IP=IP;
        mutex.unlock();
    }
}

void ClientControl::setPort(int Port)
{
    if(!connectedState)
    {
        mutex.lock();
        this->port=Port;
        mutex.unlock();
    }
}
void ClientControl::getIP()
{
    mutex.lock();
    emit sendIP(this->IP);
    mutex.unlock();
}

void ClientControl::getPort()
{
    mutex.lock();
    emit sendPort(this->port);
    mutex.unlock();
}

void ClientControl::frontCollisionOn()
{
    frontCollision=true;
}

void ClientControl::frontCollisionOff()
{
    frontCollision=false;
}

void ClientControl::rearCollisionOn()
{
    rearCollision=true;
}

void ClientControl::rearCollisionOff()
{
    rearCollision=false;
}

ClientControl::~ClientControl()
{
    m_instance=NULL;
}

