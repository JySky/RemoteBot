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

/*const int ClientCamera::SENSITIVITY_VALUE = 20;
const int ClientCamera::BLUR_SIZE = 10;
int theObject[2] = {0,0};
Rect objectBoundingRectangle = Rect(0,0,0,0);*/
const float ClientCamera::MHI_DURATION = 0.05;
const int ClientCamera::DEFAULT_THRESHOLD = 32;
const float ClientCamera::MAX_TIME_DELTA = 12500.0;
const float ClientCamera::MIN_TIME_DELTA = 5;
const int ClientCamera::visual_trackbar = 2;

//********************************** CONSTRUCTOR **********************************//

ClientCamera::ClientCamera(QObject *parent,Interface *inter) :
    QThread(parent)
{
    MainInter=inter;
    QObject::connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    QObject::connect(this, SIGNAL(connected()),MainInter, SLOT(camConnected()), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(disconnected()),MainInter, SLOT(camDisconnected()), Qt::QueuedConnection);
    //QObject::connect(this, SIGNAL(streamStopped()),MainInter, SLOT(camStreamState()), Qt::QueuedConnection);
    //QObject::connect(this, SIGNAL(frameStream(QImage)),MainInter, SLOT(setFrame(QImage)), Qt::DirectConnection);
    QObject::connect(this, SIGNAL(frameStream(QImage)),MainInter, SLOT(setFrame(QImage)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(requestSliderCamValue()),MainInter, SLOT(getSliderCamValue()), Qt::QueuedConnection);
    init();
    //edgeThresh = 1;
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
    frame2=Mat(1,1, CV_64F, cvScalar(0.));
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
            //mutex.lock();
            getImageVStream();
            //mutex.unlock();
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
    manager->get(QNetworkRequest(url));
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
    videoStreamAddress=url+"/cameras/1?q=30";///?action=stream";//"/javascript_simple.html";/cameras/1?q=30 /?action=snapshot&n=7 /?action=stream
    if(!vcap.open(videoStreamAddress.toStdString()))
    {
        qDebug() << "Error opening video stream or file";
        STOP=true;
    }
}

void ClientCamera::getImageVStream()
{
    Mat resFrame;
    //msleep(1000);
    if(!vcap.read(frame))
    {
        qDebug() << "No frame";
        STOP=true;
    }
    if(!STOP)
    {
        resFrame=frame;
        frame1=resFrame;
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
        frame2=resFrame;
        emit frameStream(getQImageFromFrame(resFrame));
    }
}

void ClientCamera::imageProcessing(bool i)
{
    if(i)
    {
        /*cedge.create(frame.size(), frame.type());
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        blur(gray, edge, Size(3,3));
        // Run the edge detector on grayscale
        Canny(edge, edge, edgeThresh, edgeThresh*3, 3);
        cedge = Scalar::all(0);
        frame.copyTo(cedge, edge);
        imshow("Edge map", cedge);
        waitKey(1);*/


        /*cvtColor(frame1,grayImage1,COLOR_BGR2GRAY);
        cvtColor(frame2,grayImage2,COLOR_BGR2GRAY);
        absdiff(grayImage1,grayImage2,differenceImage);
        //threshold(differenceImage,thresholdImage,SENSITIVITY_VALUE,255,THRESH_BINARY);
        blur(thresholdImage,thresholdImage,cv::Size(BLUR_SIZE,BLUR_SIZE));
        //threshold(thresholdImage,thresholdImage,SENSITIVITY_VALUE,255,THRESH_BINARY);
        cv::imshow("Difference Image",differenceImage);
        cv::imshow("Threshold Image", thresholdImage);
        searchForMovement(thresholdImage,frame1);
        imshow("Motion Track", frame1);*/


        updateMotionHistory(motion_mask,motion_history,timestamp,MHI_DURATION);
        calcMotionGradient(motion_history, mg_mask, mg_orient, 5, 12500.0, 3);
        segmentMotion(motion_history, seg_mask, seg_bounds, timestamp, 32);

        waitKey(1);
    }
    else if(imgProcesscreated &&!i)
    {
        cvDestroyWindow( "Motion Track" );
        cvDestroyWindow( "Difference Image" );
        cvDestroyWindow( "Threshold Image" );
    }
}

void ClientCamera::draw_motion_comp(Mat& img, int x_coordinate, int y_coordinate, int width, int height, double angle,Mat& result)
{
//	rectangle(img,Point(x_coordinate,y_coordinate), Point(x_coordinate+width,y_coordinate+width), Scalar(255,0,0), 1, 8, 0);
    int r,cx,cy;
    if(height/2 <= width/2)
        r = height/2;
    else
        r = width/2;
    cx = x_coordinate + width/2;
    cy = y_coordinate + height/2;
    angle = angle*M_PI/180;
    circle(img, Point(cx,cy), r, Scalar(255,0,0),1, 8, 0);
    line(img, Point(cx,cy), Point(int(cx+cos(angle)*r), int(cy+sin(angle)*r)), Scalar(255,0,0), 1, 8, 0);
    result = img.clone();
}
string ClientCamera::intToString(int number)
{
    stringstream ss;
    ss << number;
    return ss.str();
}

void ClientCamera::searchForMovement(Mat thresholdImage, Mat &cameraFeed)
{
    /*bool objectDetected = false;
    Mat temp;
    thresholdImage.copyTo(temp);
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );// retrieves all contours
    findContours(temp,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE );// retrieves external contours
    if(contours.size()>0)
    {
        objectDetected=true;
        qDebug()<<"plop";
    }
    else
    {
        objectDetected = false;
    }
    if(objectDetected)
    {
        vector< vector<Point> > largestContourVec;
        largestContourVec.push_back(contours.at(contours.size()-1));
        objectBoundingRectangle = boundingRect(largestContourVec.at(0));
        int xpos = objectBoundingRectangle.x+objectBoundingRectangle.width/2;
        int ypos = objectBoundingRectangle.y+objectBoundingRectangle.height/2;
        theObject[0] = xpos , theObject[1] = ypos;
    }
    int x = theObject[0];
    int y = theObject[1];
    circle(cameraFeed,Point(x,y),20,Scalar(0,255,0),2);
    line(cameraFeed,Point(x,y),Point(x,y-25),Scalar(0,255,0),2);
    line(cameraFeed,Point(x,y),Point(x,y+25),Scalar(0,255,0),2);
    line(cameraFeed,Point(x,y),Point(x-25,y),Scalar(0,255,0),2);
    line(cameraFeed,Point(x,y),Point(x+25,y),Scalar(0,255,0),2);
    putText(cameraFeed,"Tracking object at (" + intToString(x)+","+intToString(y)+")",Point(x,y),1,1,Scalar(255,0,0),2);*/
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
