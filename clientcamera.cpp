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
    STOP=false;
    bImshowcreated=false;
    bImshow=false;
    imgProcess=false;
    imgProcesscreated=false;
    QObject::connect(this, SIGNAL(connected()),MainInter, SLOT(camConnected()), Qt::DirectConnection);
    QObject::connect(this, SIGNAL(disconnected()),MainInter, SLOT(camDisconnected()), Qt::DirectConnection);
    QObject::connect(this, SIGNAL(streamStopped()),MainInter, SLOT(camStreamState()));
    QObject::connect(this, SIGNAL(frameStream(QImage)),MainInter, SLOT(setFrame(QImage)));

}

ClientCamera::~ClientCamera()
{

}

//********************************** THREAD MANAGEMENT **********************************//
void ClientCamera::run()
{
    connect();
    STOP=false;
    if(connectedState)
    {
        while(!STOP)
        {
            mutex.lock();
            getImageVStream();
            mutex.unlock();
        }
    }
}

//**********************************SLOTS **********************************//
void ClientCamera::disconnect()
{
    STOP=true;
    emit disconnected();
    connectedState=false;
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
    if(connectedState)
    {
        QNetworkAccessManager mng;
        QNetworkReply *reply =mng.get(QNetworkRequest(url));
        //mng->clearAccessCache();
        //reply =manager->get(QNetworkRequest(url));
    }
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
    videoStreamAddress=url+"/cameras/1?q=3";//"/javascript_simple.html";/cameras/1?q=30 /?action=snapshot&n=7 /?action=stream
    if(!vcap.open(videoStreamAddress.toStdString()))
    {
        qDebug() << "Error opening video stream or file";
    }
}

void ClientCamera::getImageVStream()
{
    Mat resFrame;
    if(!vcap.read(frame))
    {
        qDebug() << "No frame";
    }

    if(imgProcess)
    {
        //mutex.lock();
        imgProcesscreated=true;
        imageProcessing(true);
        //mutex.unlock();
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

   /* Mat gray;
    Mat blu;
    cvtColor(frame,gray,CV_BGR2GRAY);
    blur(gray,blu,Size(3,3));*/

    /*imshow("res2", blu);
    imshow( "result", frame );
    waitKey(1);*/

    emit frameStream(getQImageFromFrame(frame));
}

void ClientCamera::imageProcessing(bool i)
{
    if(i)
    {
        /*Mat gray;
        Mat blu;
        cvtColor(frame,gray,CV_BGR2GRAY);
        blur(gray,blu,Size(3,3));


        blur( src_gray, detected_edges, Size(3,3) );*/
        dst.create( frame.size(), frame.type() );
        cvtColor( frame, src_gray, CV_BGR2GRAY );
        /// Canny detector
        Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

        /// Using Canny's output as a mask, we display our result
        dst = Scalar::all(0);

        src.copyTo( dst, detected_edges);
        cvNamedWindow("res2", CV_WINDOW_AUTOSIZE );
        /// Create a Trackbar for user to enter threshold
        createTrackbar( "Min Threshold:", "res2", &lowThreshold, max_lowThreshold);


        //("res2", blu);
        //imshow( "res2", detected_edges );


        waitKey(1);
    }
    else if(imgProcesscreated &&!i)
    {
        cvDestroyWindow( "res2" );
    }
}

QImage ClientCamera::getQImageFromFrame(Mat f)
{
    cvtColor(f, f, CV_RGB2BGR);

    QImage *jpgImage = new QImage((uchar*) (f.data), f.cols, f.rows, f.step, QImage::Format_RGB888);

    QImage small = jpgImage->scaled(391, 291,Qt::KeepAspectRatio);

    return small;
}

QImage ClientCamera::cvMatToQImage(Mat inMat )
{
    switch ( inMat.type() )
    {
        // 8-bit, 4 channel
        case CV_8UC4:
        {
            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB32 );
            return image;
        }

        // 8-bit, 3 channel
        case CV_8UC3:
        {
            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888 );
            return image.rgbSwapped();
        }

        // 8-bit, 1 channel
        case CV_8UC1:
        {
            static QVector<QRgb>  sColorTable;
            // only create our color table once
            if ( sColorTable.isEmpty() )
            {
                for ( int i = 0; i < 256; ++i )
                    sColorTable.push_back( qRgb( i, i, i ) );
            }
            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_Indexed8 );
            image.setColorTable( sColorTable );
            return image;
        }

        default:
            qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
            break;
    }

    return QImage();
}

QPixmap ClientCamera::cvMatToQPixmap( const Mat &inMat )
{
    return QPixmap::fromImage( cvMatToQImage( inMat ) );
}

void ClientCamera::openImshow()
{
    bImshow=true;
}
void ClientCamera::closeImshow()
{
    bImshow=false;
}
