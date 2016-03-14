#include "clientcamera.h"

ClientCamera* ClientCamera::m_instance = NULL;

using namespace std;

ClientCamera::ClientCamera(Interface *inter)
{
    MainInter=inter;
    port=8080;
    IP="192.168.1.106";
}

void ClientCamera::connecttoCamera()
{
    //soc.connectToHost(IP,port);
    //MainInter->getUi()->WebView->load(QUrl("http://192.168.1.106:8080/javascript_simple.html"));
    /*if(!soc.waitForConnected(5000))
    {
        MainInter->setcolorConnected("red");
        connected=false;
    }
    else
    {
        MainInter->setcolorConnected("green");
        connected=true;
    }*/
}

void ClientCamera::setIp(QString i)
{
    IP = i;
}

void ClientCamera::setPort(int p)
{
    port = p;
}


