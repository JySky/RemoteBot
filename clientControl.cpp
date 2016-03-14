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
        ClientSend::getInstance(&soc);
        ClientReceive::getInstance(&soc,MainInter);
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
void ClientControl::stopConnectionRobot()
{
    soc.close();
}

ClientControl::~ClientControl()
{

}

