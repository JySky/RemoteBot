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

class ClientCamera: public QThread
{
    Q_OBJECT

    private:
        QMutex mutex;
        bool STOP;
        IplImage* crt;
        IplImage* prev;
        IplImage* output;
        VideoCapture vcap;
        Mat frame;
        QString IP;
        QTcpSocket soc;
        QNetworkAccessManager *manager;
        QNetworkReply *reply;
        int port;
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
        static const int  setVitesseEnable;
        static const int  setVitesseDisable;
        QString left;
        QString right;
        QString down;
        QString up;
        QString reset;
        QString url;
        QString videoStreamAddress;
        Poscam* position;
        Interface* MainInter;
        void initCam();
        void setVitesseVar();
        static ClientCamera* m_instance;
        ClientCamera(Interface *inter);
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
        QImage cvMatToQImage(Mat inMat );
        QPixmap cvMatToQPixmap( const Mat &inMat );



        Mat src, src_gray;
        Mat dst, detected_edges;

        int edgeThresh = 1;
        int lowThreshold;
        int const max_lowThreshold = 100;
        int ratio = 3;
        int kernel_size = 3;

    public slots:
        void disconnect();
        void openImshow();
        void closeImshow();
        void startImgProcess();
        void stopImgProcess();
        void stop();

    signals:
        void notConnected();
        void disconnected();
        void connected();
        void streamStopped();
        void frameStream(QImage);

    public:
        static ClientCamera* getInstance(Interface* inter)
        {
            if ( m_instance == NULL )
            {
                m_instance = new ClientCamera(inter);
            }
            return m_instance;
        }
        void run();
        void setIp(QString i);
        void setPort(int p);
        QString getIp(){return IP;}
        int getPort(){return port;}
        void setCamAuto(bool c){camAuto=c;}
        void moveCam(int pos);
};
#endif // CLIENTCAMERA_H
