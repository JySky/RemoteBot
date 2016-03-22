#include "clientcamera.h"

ClientCamera* ClientCamera::m_instance = NULL;
const int ClientCamera::camV=2;
using namespace std;

ClientCamera::ClientCamera(Interface *inter)
{
    MainInter=inter;
    port=8080;
    IP="192.168.1.106";
    connected=false;
    camAuto=false;
    reset ="/?action=command&dest=0&plugin=0&id=168062211&group=1&value=3";
    url=QString("http://"+IP+":"+port);
}

ClientCamera::~ClientCamera()
{

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
    left = QString("/?action=command&dest=0&plugin=0&id=10094852&group=1&value="+(MainInter->getSliderCam()*camV));
    right = QString("/?action=command&dest=0&plugin=0&id=10094852&group=1&value="+(MainInter->getSliderCam()*(-camV)));
    down = QString("/?action=command&dest=0&plugin=0&id=10094853&group=1&value="+(MainInter->getSliderCam()*camV));
    up = QString("/?action=command&dest=0&plugin=0&id=10094853&group=1&value="+(MainInter->getSliderCam()*(-camV)));
}
void ClientCamera::initCam()
{
    moveCam(9);
}

void ClientCamera::moveCam(int pos)
{
    setVitesseVar();
    mgr = new QNetworkAccessManager(this);
    switch(pos)
    {
        case 1://UP
            req= new QNetworkRequest(QUrl(QString(url+up)));
            mgr->get(*req);
        break;

        case 2://DOWN
            req= new QNetworkRequest(QUrl(QString(url+down)));
            mgr->get(*req);
        break;

        case 3://LEFT
            req= new QNetworkRequest(QUrl(QString(url+left)));
            mgr->get(*req);
        break;

        case 4://RIGHT
            req= new QNetworkRequest(QUrl(QString(url+right)));
            mgr->get(*req);
        break;

        case 5://UP RIGHT
            req= new QNetworkRequest(QUrl(QString(url+up)));
            mgr->get(*req);
            req= new QNetworkRequest(QUrl(QString(url+right)));
            mgr->get(*req);
        break;

        case 6://UP LEFT
            req= new QNetworkRequest(QUrl(QString(url+up)));
            mgr->get(*req);
            req= new QNetworkRequest(QUrl(QString(url+left)));
            mgr->get(*req);
        break;

        case 7://DOWN LEFT
            req= new QNetworkRequest(QUrl(QString(url+down)));
            mgr->get(*req);
            req= new QNetworkRequest(QUrl(QString(url+left)));
            mgr->get(*req);
        break;

        case 8://DOWN RIGHT
            req= new QNetworkRequest(QUrl(QString(url+down)));
            mgr->get(*req);
            req= new QNetworkRequest(QUrl(QString(url+right)));
            mgr->get(*req);
        break;

        case 9://RESET
            req= new QNetworkRequest(QUrl(QString(url+reset)));
            mgr->get(*req);
        break;
    }
}
