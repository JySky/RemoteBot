#include "clientcamera.h"
#include <QDebug>

ClientCamera* ClientCamera::m_instance = NULL;
int ClientCamera::port=8080;
QString ClientCamera::IP="192.168.1.106";
const int ClientCamera::camV=2;
const int ClientCamera::camMaxUp=45;
const int ClientCamera::camMaxDown=-45;
const int ClientCamera::camMaxLeft=80;
const int ClientCamera::camMaxRight=-80;
const int ClientCamera::setVitesseEnable=1;
const int ClientCamera::setVitesseDisable=0;
const int ClientCamera::horizontalRatio=58;
const int ClientCamera::verticalRatio=29;


//********************************** CONSTRUCTOR **********************************//

ClientCamera::ClientCamera(QObject *parent,Interface *inter) :
    QThread(parent)
{
    MainInter=inter;
    QObject::connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    QObject::connect(this, SIGNAL(connected()),MainInter, SLOT(camConnected()), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(disconnected()),MainInter, SLOT(camDisconnected()), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(frameStream(QImage)),MainInter, SLOT(setFrame(QImage)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(requestSliderCamValue()),MainInter, SLOT(getSliderCamValue()), Qt::QueuedConnection);
    init();
}

ClientCamera::~ClientCamera()
{
    m_instance=NULL;
}

void ClientCamera::init()
{
    connectedState=false;
    camAuto=false;
    manager=NULL;
    reset ="/?action=command&dest=0&plugin=0&id=168062211&group=1&value=3";
    url=QString("http://"+IP+":"+QString::number(port));
    position =new Poscam;
    position->x=0;
    position->y=0;
    sliderCamValue=40;
    STOP=false;
    bImshowcreated=false;
    bImshow=false;
    imgProcess=false;
    imgProcesscreated=false;
}

//********************************** THREAD MANAGEMENT **********************************//
void ClientCamera::run()
{
    connect();
    //msleep(1000);
    STOP=false;
    if(connectedState)
    {
        while(!STOP)
        {
            getImageVStream();
            msleep(25);
        }
    }
    vcap.release();
    m_instance=NULL;
}

//**********************************SLOTS **********************************//
void ClientCamera::receiveSliderCamValue(int val)
{
    sliderCamValue=val;
}


void ClientCamera::disconnect()
{
    STOP=true;
    emit disconnected();
    connectedState=false;
}

void ClientCamera::cameraStart()
{
    this->start();
}

void ClientCamera::connect()
{
    manager=new QNetworkAccessManager();
    manager->connectToHost(IP,port);

    if(!manager->networkAccessible())
    {
        connectedState=false;
        emit notConnected();
    }
    else
    {
        connectedState=true;
        emit connected();
        initCam();
        openImageVStream();
    }
}

void ClientCamera::stop()
{
    STOP=true;
}
void ClientCamera::startImgProcess()
{
    imgProcess=true;
}

void ClientCamera::stopImgProcess()
{
    imgProcess=false;
}

void ClientCamera::setIP(QString IP)
{
    if(!connectedState)
    {
        mutex.lock();
            this->IP=IP;
        mutex.unlock();
    }
}

void ClientCamera::setPort(int Port)
{
    if(!connectedState)
    {
        mutex.lock();
            this->port=Port;
        mutex.unlock();
    }
}
void ClientCamera::getIP()
{
    mutex.lock();
        emit sendIP(this->IP);
    mutex.unlock();
}

void ClientCamera::getPort()
{
    mutex.lock();
        emit sendPort(this->port);
    mutex.unlock();
}

//********************************** CAMERA MOVE **********************************//

void ClientCamera::initCam()
{
    manager=new QNetworkAccessManager();
    moveCam(9);
}

void ClientCamera::moveCam(int pos)
{
    if(connectedState)
    {
        setVitesseVar();
        emit requestSliderCamValue();
        switch(pos)
        {
            case 1://UP
                urlAccess(QString(url+up));
                position->y+=(sliderCamValue*camV)/verticalRatio;
            break;

            case 2://DOWN
                urlAccess(QString(url+down));
                position->y-=(sliderCamValue*camV)/verticalRatio;
            break;

            case 3://LEFT
                urlAccess(QString(url+left));
                position->x+=(sliderCamValue*camV)/horizontalRatio;
            break;

            case 4://RIGHT
                urlAccess(QString(url+right));
                position->x-=(sliderCamValue*camV)/horizontalRatio;
            break;

            case 5://UP RIGHT
                urlAccess(QString(url+up));
                urlAccess(QString(url+right));
                position->x-=(sliderCamValue*camV)/horizontalRatio;
                position->y+=(sliderCamValue*camV)/verticalRatio;
            break;

            case 6://UP LEFT
                urlAccess(QString(url+up));
                urlAccess(QString(url+left));
                position->x+=(sliderCamValue*camV)/horizontalRatio;
                position->y+=(sliderCamValue*camV)/verticalRatio;
            break;

            case 7://DOWN LEFT
                urlAccess(QString(url+down));
                urlAccess(QString(url+left));
                position->x+=(sliderCamValue*camV)/horizontalRatio;
                position->y-=(sliderCamValue*camV)/verticalRatio;
            break;

            case 8://DOWN RIGHT
                urlAccess(QString(url+down));
                urlAccess(QString(url+right));
                position->x-=(sliderCamValue*camV)/horizontalRatio;
                position->y-=(sliderCamValue*camV)/verticalRatio;
            break;

            case 9://RESET
                urlAccess(QString(url+reset));
                position->x=0;
                position->y=0;
            break;
        }
    }
}

void ClientCamera::urlAccess(QString url)
{
    msleep(20);
    manager=new QNetworkAccessManager();
    manager->connectToHost(IP,port);
    manager->get(QNetworkRequest(url));
    manager=NULL;
}

bool ClientCamera::urlAccessState(QString url)
{
    bool res=false;
    QNetworkAccessManager* mng = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    QNetworkReply *reply =mng->get(request);
    if(reply!=NULL)
    {
        res=true;
    }
    return res;
}


//********************************** SET GET **********************************//

void  ClientCamera::setUrlInit()
{
    url=QString("http://"+IP+":"+QString::number(port));
}

void ClientCamera::setVitesseVar()
{
    int valx = setVitesseEnable;
    int valy = setVitesseEnable;
    emit requestSliderCamValue();
    if((position->x>camMaxUp) || (((position->x+(sliderCamValue*camV))/horizontalRatio)>camMaxUp)||(position->x<camMaxDown) || (((position->x-(sliderCamValue*camV))/horizontalRatio)<camMaxDown))
    {
        valx=setVitesseDisable;
    }
    else if((position->y>camMaxLeft) || (((position->y+(sliderCamValue*camV))/verticalRatio)>camMaxLeft)||(position->y<camMaxRight) || (((position->y-(sliderCamValue*camV))/verticalRatio)<camMaxRight))
    {
        valy=setVitesseDisable;
    }

    //value horizontal 58->1°  189
    //value vertical 29->1°  109
    left = QString("/?action=command&dest=0&plugin=0&id=10094852&group=1&value="+QString::number((sliderCamValue*camV*valx)));
    right = QString("/?action=command&dest=0&plugin=0&id=10094852&group=1&value="+QString::number((sliderCamValue*(-camV)*valx)));
    down = QString("/?action=command&dest=0&plugin=0&id=10094853&group=1&value="+QString::number((sliderCamValue*camV*valy)));
    up = QString("/?action=command&dest=0&plugin=0&id=10094853&group=1&value="+QString::number((sliderCamValue*(-camV)*valy)));
}


//*********************************** IMAGE PROCESSING ***********************************//

void ClientCamera::openImageVStream()
{
    url=QString("http://"+IP+":"+QString::number(port));
    videoStreamAddress=url+"/?action=stream";///?action=stream";//"/javascript_simple.html";/cameras/1?q=30 /?action=snapshot&n=7 /?action=stream
    if(!vcap.open(videoStreamAddress.toStdString()))
    {
        qDebug() << "Error opening video stream or file";
        STOP=true;
    }
}

void ClientCamera::getImageVStream()
{
    Mat resFrame;
    msleep(20);
    if(!vcap.read(frame))
    {
        qDebug() << "No frame";
        STOP=true;
    }
    if(!STOP)
    {
        resFrame=frame;
        if(imgProcess)
        {
            imgProcesscreated=true;
            imageProcessing(true);
        }else
        {
            imageProcessing(false);
            imgProcesscreated=false;
        }
        if(bImshow)
        {
            cvNamedWindow("video", 0 );
            bImshowcreated=true;
            imshow("video", frame );
            waitKey(1);
        }
        else if (bImshowcreated)
        {
            cvDestroyWindow( "video" );
            bImshowcreated=false;
        }
        emit frameStream(getQImageFromFrame(resFrame));
    }
}

void ClientCamera::imageProcessing(bool i)
{
    if(i)
    {
        vector<Mat> colors;
        Mat resframe = frame;
        Mat outputframe;
        split(resframe, colors);
        equalizeHist(colors[0], colors[0]);
        equalizeHist(colors[1], colors[1]);
        equalizeHist(colors[2], colors[2]);
        merge(colors, outputframe);
        imshow("Night Vision", resframe);

        waitKey(1);
    }
    else if(imgProcesscreated &&!i)
    {
        cvDestroyWindow( "Night Vision" );
    }
}

QImage ClientCamera::getQImageFromFrame(Mat f)
{
    cvtColor(f, f, CV_RGB2BGR);

    /*QImage *jpgImage = new QImage((uchar*) (f.data), f.cols, f.rows, f.step, QImage::Format_RGB888);
    QImage small = jpgImage->scaled(391, 291,Qt::KeepAspectRatio);*/

    return QImage((uchar*) (f.data), f.cols, f.rows, f.step, QImage::Format_RGB888);//small;
}

void ClientCamera::openImshow()
{
    bImshow=true;
}
void ClientCamera::closeImshow()
{
    bImshow=false;
}
