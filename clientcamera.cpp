#include "clientcamera.h"

ClientCamera* ClientCamera::m_instance = NULL;

using namespace std;

ClientCamera::ClientCamera(Interface *inter)
{
    MainInter=inter;
    port=8080;
    IP="192.168.1.106";

}
ClientCamera::~ClientCamera()
{

}

void ClientCamera::stopConnectionCamera()
{
    //soc.close();
    //MainInter->setQWebView("");
}

bool ClientCamera::connecttoCamera()
{
    bool res;
    res=true;
        //MainInter->setQWebView("http://"+IP+":"+port+"/javascript_simple.html");
    return res;
}

void ClientCamera::setIp(QString i)
{
    IP = i;
}

void ClientCamera::setPort(int p)
{
    port = p;
}


