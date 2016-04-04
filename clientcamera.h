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
#include <QTimer>

class Interface;

struct Poscam
{
    int x;
    int y;
};

using namespace cv;
class ClientCamera: public QObject
{
    Q_OBJECT

    private:

        IplImage* crt;
        IplImage* prev;
        IplImage* output;
        QTimer timer;
        VideoCapture vcap;
        Mat frame;
        Mat prevFrame;
        QString IP;
        QTcpSocket soc;
        QNetworkAccessManager manager;
        int port;
        bool connectedState;
        bool fisrtframe;
        bool camAuto;
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

    private slots:
        void disconnect();
        void connect();
        void getImageVStream();

    signals:
        void disconnected();
        void connected();
        void streamStopped();

    public:
        static ClientCamera* getInstance(Interface* inter)
        {
            if ( m_instance == NULL )
            {
                m_instance = new ClientCamera(inter);
            }
            return m_instance;
        }

        void setIp(QString i);
        void setPort(int p);
        QString getIp(){return IP;}
        int getPort(){return port;}
        void setCamAuto(bool c){camAuto=c;}
        void moveCam(int pos);
};
#endif // CLIENTCAMERA_H
