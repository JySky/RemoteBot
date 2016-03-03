#include "clientcontrol.h"

ClientControl* ClientControl::m_instance = NULL;

using namespace std;

ClientControl::ClientControl(Interface *inter)
{
    MainInter=inter;
    port=15020;
    this->IP="192.168.1.106";
    rightSpeed=0;
    leftSpeed=0;
    rightSpeedFlag=0;
    leftSpeedFlag=0;
    leftSpeedLoop=0;
    rightSpeedLoop=0;
    connected=false;
}

void ClientControl::receive()
{
    QByteArray buffer;
    buffer=soc.readAll();

    //int rcvnbr = recv(socket_handler,sbuf,21,0);
   /* dataL->SpeedFront=(int)((sbuf[1] << 8) + sbuf[0]);
    if (dataL->SpeedFront > 32767)
        dataL->SpeedFront=dataL->SpeedFront-65536;
    dataL->BatLevel=sbuf[2];
    dataL->IR=sbuf[3];
    dataL->IR2=sbuf[4];
    dataL->odometry=((((long)sbuf[8] << 24))+(((long)sbuf[7] <<16))+(((long)sbuf[6] << 8))+((long)sbuf[5]));
    dataR->SpeedFront=(int)(sbuf[10] << 8) + sbuf[9];
    if (dataR->SpeedFront > 32767)
        dataR->SpeedFront=dataR->SpeedFront-65536;
    dataR->BatLevel=0;
    dataR->IR=sbuf[11];
    dataR->IR2=sbuf[12];
    dataR->odometry=((((long)sbuf[16] << 24))+(((long)sbuf[15] <<16))+(((long)sbuf[14] << 8))+((long)sbuf[13]));
    dataL->Current=sbuf[17];
    dataR->Current=sbuf[17];
    dataL->Version=sbuf[18];
    dataR->Version=sbuf[18];*/
}

void ClientControl::init()
{
    QByteArray bufsend;

   /* bufsend[0]=(-speedlab);
    bufsend[1]=(-speedlab >> 8); //speed is a short and it is tics / 50 ms
    bufsend[2]=(unsigned char)(tmpadc2 >> 2);//Bat Volt:10.1V 1.28V 404/4->101
    bufsend[3]=(unsigned char)(tmpadc4 >> 2);//3.3v->255 2v-> 624/4 -> 156
    bufsend[4]=(unsigned char)(tmpadc3 >> 2);//3.3v->255 2v-> 624/4 -> 156
    bufsend[5]=bufposition[0];//Acumulated odometrie is a float
    bufsend[6]=bufposition[1];//12ppr x 4 x 51 gear box = 2448 tics/wheel turn
    bufsend[7]=bufposition[2];
    bufsend[8]=bufposition[3];
    bufsend[9]=(speedlab2);
    bufsend[10]=(speedlab2 >> 8);
    bufsend[11]=(unsigned char)(tmpadc0 >> 2);
    bufsend[12]=(unsigned char)(tmpadc1 >> 2);
    bufsend[13]=bufposition2[0];
    bufsend[14]=bufposition2[1];
    bufsend[15]=bufposition2[2];
    bufsend[16]=bufposition2[3];
    bufsend[17]=0;//robot current // *0.194-37.5 = I in Amp / * 10 for the GUI : 10 -> 1 A (ACS712 30Amp chip) // *0.129-25 =I (for ACS712 20A chip)
    bufsend[18]=14; //firmware version
    // bufsend[19]=crc16 low;
    // bufsend[20]=crc16 high;*/
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
       /* QMessageBox msgBox;
        QMessageBox::warning(msgBox,"Connection Failed","Connection failed on : "+IP+":"+port);*/
        /*cout<<"fail";*/
        //emit valueChanged(false);
        MainInter->setStyleSheet("QLabel#colorConnected { background-color : red;}");
        connected=false;

    }
    else
    {
        MainInter->setStyleSheet("QLabel#colorConnected  { background-color : green;}");
        connected=true;
    }
   // QObject::connect(m_instance, SIGNAL(valueChanged(bool)),MainInter, SLOT(connectionState(bool)));
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

