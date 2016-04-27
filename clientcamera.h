#ifndef CLIENTCAMERA_H
#define CLIENTCAMERA_H
#include <QtNetwork>
#include <iostream>
#include <QMessageBox>
#include "interface.h"
#include <QMainWindow>
#include <QObject>
#include <QMessageBox>
#include <QInputDialog>
#include <QIcon>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/types_c.h>
#include <QThread>
#include <QMutex>
#include <QImage>
#include <QPixmap>
#include <QtNetwork/QHttpPart>


class Interface;

struct Poscam
{
    int x;
    int y;
};

using namespace cv;
using namespace std;

class ClientCamera: public QThread
{
    Q_OBJECT

    private:
        ClientCamera(QObject *parent,Interface *inter);
        QMutex mutex;
        bool STOP;
        IplImage* crt;
        IplImage* prev;
        IplImage* output;
        VideoCapture vcap;
        Mat frame;
        static QString IP;
        QTcpSocket soc;
        QNetworkAccessManager *manager;
        static int port;
        bool connectedState;
        bool camAuto;
        bool imgProcess;
        bool bImshow;
        bool bImshowcreated;
        bool imgProcesscreated;
        static const int camV;
        static const int camMaxUp;
        static const int camMaxDown;
        static const int camMaxLeft;
        static const int camMaxRight;
        static const int setVitesseEnable;
        static const int setVitesseDisable;
        static const int horizontalRatio;
        static const int verticalRatio;
        QString left;
        QString right;
        QString down;
        QString up;
        QString reset;
        QString url;
        QString videoStreamAddress;
        Poscam* position;
        Interface* MainInter;
        int sliderCamValue;
        void initCam();
        void setVitesseVar();
        static ClientCamera* m_instance;

        ~ClientCamera();
        void urlAccess(QString url);
        bool urlAccessState(QString url);
        void setUrlInit();
        void processing();
        void openImageVStream();
        QImage getQImageFromFrame(Mat f);
        void connect();
        void getImageVStream();
        void imageProcessing(bool i);
        void init();

    public slots:
        void receiveSliderCamValue(int val);
        void setIP(QString IP);
        void setPort(int Port);
        void getIP();
        void getPort();
        void disconnect();
        void openImshow();
        void closeImshow();
        void startImgProcess();
        void stopImgProcess();
        void stop();
        void cameraStart();
        void moveCam(int pos);

    signals:
        void notConnected();
        void disconnected();
        void connected();
        void streamStopped();
        void frameStream(QImage);
        void sendIP(QString);
        void sendPort(int);
        void requestSliderCamValue();


    public:

        static ClientCamera* getInstance(){return m_instance;}
        static ClientCamera* getInstance(QObject* parent, Interface* inter)
        {
            if ( m_instance == NULL )
            {
                m_instance = new ClientCamera(parent,inter);
            }
            return m_instance;
        }
        void run();

};
#endif // CLIENTCAMERA_H
