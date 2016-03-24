#include "clientcamera.h"
#include <QDebug>

ClientCamera* ClientCamera::m_instance = NULL;
const int ClientCamera::camV=2;
const int ClientCamera::camMaxUp=51;
const int ClientCamera::camMaxDown=-51;
const int ClientCamera::camMaxLeft=94;
const int ClientCamera::camMaxRight=-94;
const int ClientCamera::setVitesseEnable=1;
const int ClientCamera::setVitesseDisable=0;

using namespace std;

ClientCamera::ClientCamera(Interface *inter)
{
    MainInter=inter;
    port=8080;
    IP="192.168.1.106";
    connected=false;
    camAuto=false;
    reset ="/?action=command&dest=0&plugin=0&id=168062211&group=1&value=3";
    url=QString("http://"+IP+":"+QString::number(port));
    position =new Poscam;
    position->x=0;
    position->y=0;
    connect(&soc, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(&soc, SIGNAL(disconnected()),MainInter, SLOT(camDisconnected()));
}

ClientCamera::~ClientCamera()
{

}

void ClientCamera::disconnected()
{
    soc.close();
    MainInter->setQWebView("");
    MainInter->setImage(":/image/image/nosignal.png");
    connected=false;
    if(soc.state()==QAbstractSocket::UnconnectedState)
    {
        std::string s ="Disconnected from"+IP.toStdString()+"/"+(QString::number(port)).toStdString();
        const char* msg=s.c_str();
        QMessageBox::information(
              MainInter,
              tr("Camera"),
              tr(msg) );
    }
    MainInter->setcolorConnected("red");
}

void ClientCamera::stopConnectionCamera()
{
    initCam();
    connected=false;
    soc.close();
    MainInter->setQWebView("");
    MainInter->setImage(":/image/image/nosignal.png");
}

bool ClientCamera::connecttoCamera()
{
    soc.connectToHost(IP,port);

    if(!soc.waitForConnected(5000))
    {
        connected=false;
    }
    else
    {
        connected=true;
        MainInter->setImage("");
        initCam();
        MainInter->setQWebView(url+"/javascript_simple.html");
    }
    if(soc.state()==QAbstractSocket::UnconnectedState)
    {
        std::string s ="Not connected to"+IP.toStdString()+"/"+(QString::number(port)).toStdString();
        const char* msg=s.c_str();
        QMessageBox::critical(
              MainInter,
              tr("Camera"),
              tr(msg) );
    }
    return connected;
}

void ClientCamera::setIp(QString i)
{
    IP = i;
}

void ClientCamera::setPort(int p)
{
    port = p;
}

void ClientCamera::setVitesseVar()
{
    int valx = setVitesseEnable;
    int valy = setVitesseEnable;

    if((position->x>camMaxUp) || ((position->x+(MainInter->getSliderCam()*camV))>camMaxUp)||(position->x<camMaxDown) || ((position->x+(MainInter->getSliderCam()*(-camV)))<camMaxDown))
    {
        valx=setVitesseDisable;
    }
    else if((position->y>camMaxLeft) || ((position->y+(MainInter->getSliderCam()*camV))>camMaxLeft)||(position->y<camMaxRight) || ((position->y+(MainInter->getSliderCam()*(-camV)))<camMaxRight))
    {
        valy=setVitesseDisable;
    }
    left = QString("/?action=command&dest=0&plugin=0&id=10094852&group=1&value="+QString::number((MainInter->getSliderCam()*camV*valx)));
    right = QString("/?action=command&dest=0&plugin=0&id=10094852&group=1&value="+QString::number((MainInter->getSliderCam()*(-camV)*valx)));
    down = QString("/?action=command&dest=0&plugin=0&id=10094853&group=1&value="+QString::number((MainInter->getSliderCam()*camV*valy)));
    up = QString("/?action=command&dest=0&plugin=0&id=10094853&group=1&value="+QString::number((MainInter->getSliderCam()*(-camV)*valy)));
}

void ClientCamera::initCam()
{
    moveCam(9);
}

void ClientCamera::moveCam(int pos)
{
    setVitesseVar();
    switch(pos)
    {
        case 1://UP
            urlAccess(QString(url+up));
            position->y+=3*camV;
        break;

        case 2://DOWN
            urlAccess(QString(url+down));
            position->y-=3*camV;
        break;

        case 3://LEFT
            urlAccess(QString(url+left));
            position->x+=5*camV;
        break;

        case 4://RIGHT
            urlAccess(QString(url+right));
            position->x-=5*camV;
        break;

        case 5://UP RIGHT
            urlAccess(QString(url+up));
            urlAccess(QString(url+right));
            position->x-=5*camV;
            position->y+=3*camV;
        break;

        case 6://UP LEFT
            urlAccess(QString(url+up));
            urlAccess(QString(url+left));
            position->x+=5*camV;
            position->y+=3*camV;
        break;

        case 7://DOWN LEFT
            urlAccess(QString(url+down));
            urlAccess(QString(url+left));
            position->x+=5*camV;
            position->y-=3*camV;
        break;

        case 8://DOWN RIGHT
            urlAccess(QString(url+down));
            urlAccess(QString(url+right));
            position->x-=5*camV;
            position->y-=3*camV;
        break;

        case 9://RESET
            urlAccess(QString(url+reset));
            position->x=0;
            position->y=0;
        break;
    }
}

void ClientCamera::urlAccess(QString url)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    QNetworkReply *reply = manager->get(request);
}
