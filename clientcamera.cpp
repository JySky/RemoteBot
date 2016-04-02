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
    connectedState=false;
    camAuto=false;
    reset ="/?action=command&dest=0&plugin=0&id=168062211&group=1&value=3";
    url=QString("http://"+IP+":"+QString::number(port));
    position =new Poscam;
    position->x=0;
    position->y=0;
    fisrtframe=true;
    QObject::connect(this, SIGNAL(connected()),MainInter, SLOT(camConnected()));
    QObject::connect(this, SIGNAL(disconnected()),MainInter, SLOT(camDisconnected()));
    QObject::connect(this, SIGNAL(streamStopped()),MainInter, SLOT(camStreamState()));
}

ClientCamera::~ClientCamera()
{

}

void ClientCamera::processing()
{
    timer.setInterval(40);
    QObject::connect(&timer, SIGNAL (timeout()), this, SLOT (getImageVStream()));
    timer.start();
}

void ClientCamera::disconnect()
{
    timer.stop();
    emit disconnected();
    MainInter->setImage(":/image/image/nosignal.png");
    connectedState=false;
    if(!connectedState)
    {
        std::string s ="Disconnected from"+IP.toStdString()+"/"+(QString::number(port)).toStdString();
        const char* msg=s.c_str();
        QMessageBox::information(
              MainInter,
              tr("Camera"),
              tr(msg) );
    }
}

void ClientCamera::connect()
{
    manager.connectToHost(IP,port);

    if(!manager.networkAccessible())
    {
        connectedState=false;
    }
    else
    {
        connectedState=true;
        MainInter->setImage("");
        emit connected();
        initCam();
        openImageVStream();
        processing();
    }

    if(!connectedState)
    {
        std::string s ="Not connected to"+IP.toStdString()+"/"+(QString::number(port)).toStdString();
        const char* msg=s.c_str();
        QMessageBox::critical(
              MainInter,
              tr("Camera"),
              tr(msg) );
    }
}

//********************************** CAMERA MOVE **********************************//

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
    QNetworkAccessManager* mng = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    QNetworkReply *reply =mng->get(request);
    //mng->clearAccessCache();
}

bool ClientCamera::urlAccessState(QString url)
{
    bool res=false;
    QNetworkAccessManager* mng = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    QNetworkReply *reply =mng->get(request);//QNetworkReply *reply =
    //mng->clearAccessCache();
    if(reply!=NULL)
    {
        res=true;
    }
    return res;
}


//********************************** SET GET **********************************//



void ClientCamera::setIp(QString i)
{
    IP = i;
    setUrlInit();
}

void ClientCamera::setPort(int p)
{
    port = p;
    setUrlInit();
}

void  ClientCamera::setUrlInit()
{
    url=QString("http://"+IP+":"+QString::number(port));
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


//*********************************** IMAGE PROCESSING ***********************************//

void ClientCamera::openImageVStream()
{
    url=QString("http://"+IP+":"+QString::number(port));
    videoStreamAddress=url+"/?action=stream";//"/javascript_simple.html";/cameras/1?q=30 /?action=snapshot&n=7 /?action=stream
    if(!vcap.open(videoStreamAddress.toStdString()))
    {
        qDebug() << "Error opening video stream or file";
    }
    /*frame= Mat(1,1, CV_64F, cvScalar(0.));
    prevFrame = Mat(1,1, CV_64F, cvScalar(0.));*/
}

void ClientCamera::getImageVStream()
{
    if(!vcap.read(frame))
    {
        qDebug() << "No frame";
    }
    Mat gray;
    Mat blu;
    cvtColor(frame,gray,CV_BGR2GRAY);
    blur(gray,blu,Size(3,3));

    imshow("res2", blu);
    imshow( "result", frame );
    waitKey(1);
    /*if(fisrtframe)
    {
        fisrtframe=false;
        output = cvCreateImage(cvSize(frame.cols, frame.rows), IPL_DEPTH_8U, 1);
        prev = cvCreateImage(cvSize(frame.cols, frame.rows), IPL_DEPTH_8U, 3);
        cvZero(prev);
    }*/

    /*IplImage ipltemp = frame;
    cvCopy(&ipltemp, crt);*/

    //motionDetection(crt,prev,output,60);
    //frame=cvarrToMat(output);
    //prevFrame=cvarrToMat(output);

    MainInter->setImage(getQImageFromFrame(frame));
}

void ClientCamera::motionDetection(IplImage* crt_img, IplImage* prev_img, IplImage* output_img, int seuil)
{
    IplImage* img_prev=cvCreateImage(cvGetSize(prev_img),IPL_DEPTH_8U,3);
    IplImage* img_crt=cvCreateImage(cvGetSize(crt_img),IPL_DEPTH_8U,3);

    cvCvtColor(crt_img,output_img,CV_RGB2GRAY);

    cvCopy(prev_img,img_prev, NULL);
    cvCopy(crt_img,img_crt, NULL);

    GrayImage img_w(img_crt);
    GrayImage img_fw(img_prev);
    GrayImage img_t(output_img);

    CvSize size = cvGetSize(prev_img);

    for(int i=0; i <size.width;i++)
    {
        for(int j=0; j < size.height;j++)
        {
            int a = img_w[j][i];
            int b = img_fw[j][i];
            if( a-b>seuil)
            {
                img_t[j][i] = 255;
            }
            else
            {
                img_t[j][i] = 0;
            }
        }
    }
}

QImage ClientCamera::getQImageFromFrame(Mat f)
{
    cvtColor(f, f, CV_RGB2BGR);

    QImage *jpgImage = new QImage((uchar*) (f.data), f.cols, f.rows, f.step, QImage::Format_RGB888);

    QImage small = jpgImage->scaled(391, 291,Qt::KeepAspectRatio);

    return small;
}
