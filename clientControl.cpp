#include "clientcontrol.h"

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
}

void ClientControl::send()
{
    soc.write(control());
}

QByteArray ClientControl::control()
{
    QByteArray toSend;
    unsigned char char7=0;
    toSend.append((unsigned char)255);//char1
    toSend.append((unsigned char)7);//char2
    toSend.append(leftSpeed);//char3
    toSend.append(leftSpeed);//char4
    toSend.append(rightSpeed);//char5
    toSend.append(rightSpeed);//char6
    //char7 build
    char7+=128*leftSpeedLoop;
    char7+=64*leftSpeedFlag;
    char7+=32*leftSpeedLoop;
    char7+=16*rightSpeedFlag;
    char7+=8*0;
    toSend.append(char7);//char7
    return toSend;
}

void ClientControl::connecttoRobot()
{
    soc.connectToHost(IP,port);

    if(!soc.waitForConnected(5000))
    {
        MainInter->setcolorConnected("red");
        connected=false;
    }
    else
    {
        MainInter->setcolorConnected("green");
        connected=true;
    }
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

ClientControl::~ClientControl()
{

}

