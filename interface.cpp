#include "interface.h"
#include "ui_interface.h"
#include <QKeyEvent>



Interface::Interface(QWidget *parent) : QMainWindow(parent), ui(new Ui::Interface)
{
    ui->setupUi(this);
    Clientcont = ClientControl::getInstance(this);
    Clientcam = ClientCamera::getInstance(this);
}


void Interface::ControlDirection()
{
    if((!Kpressed && !Lpressed && !Ipressed && !Jpressed)||(Kpressed && !Lpressed && Ipressed && !Jpressed)||(!Kpressed && Lpressed && !Ipressed && Jpressed))
    { // stop
        Clientcont->setLeftSpeedFlag(1);
        Clientcont->setRightSpeedFlag(1);
        Clientcont->setLeftSpeed((int)(ui->displaySpeedSet->value()*0));
        Clientcont->setRightSpeed((int)(ui->displaySpeedSet->value()*0));
        QPixmap mypix (":/image/image/arrowUp.png");
        ui->robotUp->setPixmap(mypix);
        QPixmap mypix1 (":/image/image/arrowLeft.png");
        ui->robotLeft->setPixmap(mypix1);
        QPixmap mypix2 (":/image/image/arrowRight.png");
        ui->robotRight->setPixmap(mypix2);
        QPixmap mypix3 (":/image/image/arrowDown.png");
        ui->robotDown->setPixmap(mypix3);
    }
    else if(Ipressed && !Jpressed && !Kpressed && !Lpressed)
    { // avant
        Clientcont->setLeftSpeedFlag(1); // mise en route gauche
        Clientcont->setRightSpeedFlag(1); // mise en route droit
        Clientcont->setLeftSpeed((int)(ui->displaySpeedSet->value()*2.4)); // vitesse gauche
        Clientcont->setRightSpeed((int)(ui->displaySpeedSet->value()*2.4)); // vitesse droit
        QPixmap mypix (":/image/image/arrowUpPressed.png");
        ui->robotUp->setPixmap(mypix);
        QPixmap mypix1 (":/image/image/arrowLeft.png");
        ui->robotLeft->setPixmap(mypix1);
        QPixmap mypix2 (":/image/image/arrowRight.png");
        ui->robotRight->setPixmap(mypix2);
        QPixmap mypix3 (":/image/image/arrowDown.png");
        ui->robotDown->setPixmap(mypix3);
    }
    else if(Kpressed && !Ipressed && !Jpressed && !Lpressed)
    { //arrière
        Clientcont->setLeftSpeedFlag(0);
        Clientcont->setRightSpeedFlag(0);
        Clientcont->setLeftSpeed((int)(ui->displaySpeedSet->value()*2.4));
        Clientcont->setRightSpeed((int)(ui->displaySpeedSet->value()*2.4));
        QPixmap mypix (":/image/image/arrowUp.png");
        ui->robotUp->setPixmap(mypix);
        QPixmap mypix1 (":/image/image/arrowLeft.png");
        ui->robotLeft->setPixmap(mypix1);
        QPixmap mypix2 (":/image/image/arrowRight.png");
        ui->robotRight->setPixmap(mypix2);
        QPixmap mypix3 (":/image/image/arrowDownPressed.png");
        ui->robotDown->setPixmap(mypix3);
    }
    else if(Jpressed && !Ipressed && !Kpressed && !Lpressed)
    { // gauche
        Clientcont->setLeftSpeedFlag(1);
        Clientcont->setRightSpeedFlag(0);
        Clientcont->setLeftSpeed((int)(ui->displaySpeedSet->value()*2.4));
        Clientcont->setRightSpeed((int)(ui->displaySpeedSet->value()*2.4));
        QPixmap mypix (":/image/image/arrowUp.png");
        ui->robotUp->setPixmap(mypix);
        QPixmap mypix1 (":/image/image/arrowLeftPressed.png");
        ui->robotLeft->setPixmap(mypix1);
        QPixmap mypix2 (":/image/image/arrowRight.png");
        ui->robotRight->setPixmap(mypix2);
        QPixmap mypix3 (":/image/image/arrowDown.png");
        ui->robotDown->setPixmap(mypix3);
    }
    else if(Lpressed && !Ipressed && !Jpressed && !Kpressed)
    { // droite
        Clientcont->setLeftSpeedFlag(0);
        Clientcont->setRightSpeedFlag(1);
        Clientcont->setLeftSpeed((int)(ui->displaySpeedSet->value()*2.4));
        Clientcont->setRightSpeed((int)(ui->displaySpeedSet->value()*2.4));
        QPixmap mypix (":/image/image/arrowUp.png");
        ui->robotUp->setPixmap(mypix);
        QPixmap mypix1 (":/image/image/arrowLeft.png");
        ui->robotLeft->setPixmap(mypix1);
        QPixmap mypix2 (":/image/image/arrowRightPressed.png");
        ui->robotRight->setPixmap(mypix2);
        QPixmap mypix3 (":/image/image/arrowDown.png");
        ui->robotDown->setPixmap(mypix3);
    }
    else if(Jpressed && Ipressed && !Kpressed && !Lpressed)
    { // avant gauche
        Clientcont->setLeftSpeedFlag(1);
        Clientcont->setRightSpeedFlag(1);
        Clientcont->setLeftSpeed((int)(ui->displaySpeedSet->value()*1.4));
        Clientcont->setRightSpeed((int)(ui->displaySpeedSet->value()*2.4));
        QPixmap mypix (":/image/image/arrowUpPressed.png");
        ui->robotUp->setPixmap(mypix);
        QPixmap mypix1 (":/image/image/arrowLeftPressed.png");
        ui->robotLeft->setPixmap(mypix1);
        QPixmap mypix2 (":/image/image/arrowRight.png");
        ui->robotRight->setPixmap(mypix2);
        QPixmap mypix3 (":/image/image/arrowDown.png");
        ui->robotDown->setPixmap(mypix3);
    }
    else if(Lpressed && Ipressed && !Kpressed && !Jpressed)
    { // avant droite
        Clientcont->setLeftSpeedFlag(1);
        Clientcont->setRightSpeedFlag(1);
        Clientcont->setLeftSpeed((int)(ui->displaySpeedSet->value()*2.4));
        Clientcont->setRightSpeed((int)(ui->displaySpeedSet->value()*1.4));
        QPixmap mypix (":/image/image/arrowUpPressed.png");
        ui->robotUp->setPixmap(mypix);
        QPixmap mypix1 (":/image/image/arrowLeft.png");
        ui->robotLeft->setPixmap(mypix1);
        QPixmap mypix2 (":/image/image/arrowRightPressed.png");
        ui->robotRight->setPixmap(mypix2);
        QPixmap mypix3 (":/image/image/arrowDown.png");
        ui->robotDown->setPixmap(mypix3);
    }
    else if(Kpressed && Jpressed && !Ipressed && !Lpressed)
    { // arrière gauche
        Clientcont->setLeftSpeedFlag(1);
        Clientcont->setRightSpeedFlag(1);
        Clientcont->setLeftSpeed((int)(ui->displaySpeedSet->value()*1.4));
        Clientcont->setRightSpeed((int)(ui->displaySpeedSet->value()*2.4));
        QPixmap mypix (":/image/image/arrowUp.png");
        ui->robotUp->setPixmap(mypix);
        QPixmap mypix1 (":/image/image/arrowLeftPressed.png");
        ui->robotLeft->setPixmap(mypix1);
        QPixmap mypix2 (":/image/image/arrowRight.png");
        ui->robotRight->setPixmap(mypix2);
        QPixmap mypix3 (":/image/image/arrowDownPressed.png");
        ui->robotDown->setPixmap(mypix3);
    }
    else if(Kpressed && Lpressed && !Ipressed && !Jpressed)
    { // arrière droit
        Clientcont->setLeftSpeedFlag(1);
        Clientcont->setRightSpeedFlag(1);
        Clientcont->setLeftSpeed((int)(ui->displaySpeedSet->value()*2.4));
        Clientcont->setRightSpeed((int)(ui->displaySpeedSet->value()*1.4));
        QPixmap mypix (":/image/image/arrowUp.png");
        ui->robotUp->setPixmap(mypix);
        QPixmap mypix1 (":/image/image/arrowLeft.png");
        ui->robotLeft->setPixmap(mypix1);
        QPixmap mypix2 (":/image/image/arrowRightPressed.png");
        ui->robotRight->setPixmap(mypix2);
        QPixmap mypix3 (":/image/image/arrowDownPressed.png");
        ui->robotDown->setPixmap(mypix3);
    }

}

void Interface::ControlCam()
{
    if(Zpressed)
    {
        req.setUrl((QUrl(QString("http://"+Clientcam->getIp()+":"+Clientcam->getPort()+"/?action=command&dest=0&plugin=0&id=10094853&group=1&value=-200"))));
        mgr.get(req);

    }
    else if(Qpressed)
    {
        req.setUrl((QUrl(QString("http://"+Clientcam->getIp()+":"+Clientcam->getPort()+"/?action=command&dest=0&plugin=0&id=10094852&group=1&value=200"))));
        mgr.get(req);
    }
    else if(Spressed)
    {
        req.setUrl((QUrl(QString("http://"+Clientcam->getIp()+":"+Clientcam->getPort()+"/?action=command&dest=0&plugin=0&id=10094853&group=1&value=200"))));
        mgr.get(req);
    }
    else if(Dpressed)
    {
        req.setUrl((QUrl(QString("http://"+Clientcam->getIp()+":"+Clientcam->getPort()+"/?action=command&dest=0&plugin=0&id=10094852&group=1&value=-200"))));
        mgr.get(req);
    }
}

void Interface::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_I:
                Ipressed=true;
            break;

        case Qt::Key_K:
                Kpressed=true;
            break;

        case Qt::Key_J:
                Jpressed=true;
            break;

        case Qt::Key_L:
                Lpressed=true;
            break;

        case Qt::Key_Z:
            Zpressed=true;
            /*req.setUrl((QUrl(QString("http://"+Clientcam->getIp()+":"+Clientcam->getPort()+"/?action=command&dest=0&plugin=0&id=10094853&group=1&value=-200"))));
            mgr.get(req);*/
            break;

        case Qt::Key_S:
            Spressed=true;
            /*req.setUrl((QUrl(QString("http://"+Clientcam->getIp()+":"+Clientcam->getPort()+"/?action=command&dest=0&plugin=0&id=10094853&group=1&value=200"))));
            mgr.get(req);*/
            break;

        case Qt::Key_Q:
            Qpressed=true;
            /*req.setUrl((QUrl(QString("http://"+Clientcam->getIp()+":"+Clientcam->getPort()+"/?action=command&dest=0&plugin=0&id=10094852&group=1&value=200"))));
            mgr.get(req);*/
            break;

        case Qt::Key_D:
            Dpressed=true;
            /*req.setUrl((QUrl(QString("http://"+Clientcam->getIp()+":"+Clientcam->getPort()+"/?action=command&dest=0&plugin=0&id=10094852&group=1&value=-200"))));
            mgr.get(req);*/
            break;
    }
    ControlDirection();
    ControlCam();
}

void Interface::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_I:
            Ipressed=false;
            break;

        case Qt::Key_K:
            Kpressed=false;
            break;

        case Qt::Key_J:
            Jpressed=false;
            break;

        case Qt::Key_L:
            Lpressed=false;
            break;

        case Qt::Key_Z:
            Zpressed=false;
            break;

        case Qt::Key_Q:
            Qpressed=false;
            break;

        case Qt::Key_S:
            Spressed=false;
            break;

        case Qt::Key_D:
            Dpressed=false;
            break;
    }
    ControlDirection();
    ControlCam();
}

void Interface::on_robotStart_clicked()
{
    bool resRobot=false;
    bool resCam=false;
    resRobot=Clientcont->connecttoRobot();
    //resCam=Clientcam->connecttoCamera();
    /*if(Clientcam->getSocket()->state()==QAbstractSocket::UnconnectedState)
    {
        std::string s ="Not connected to"+Clientcam->getIp().toStdString()+"/"+QString::number(Clientcam->getPort()).toStdString();
        const char* msg=s.c_str();
        QMessageBox::critical(
              this,
              tr("Camera"),
              tr(msg) );
        QPixmap mypix (":/image/image/nosignal.png");
        ui->frame->setPixmap(mypix);
    }*/

    /*if(Clientcont->getSocket()->state()==QAbstractSocket::UnconnectedState)
    {
        std::string s ="Not connected to"+Clientcont->getIp().toStdString()+"/"+QString::number(Clientcont->getPort()).toStdString();
        const char* msg=s.c_str();
        QMessageBox::critical(
              this,
              tr("Robot"),
              tr(msg) );
    }*/
    if(resRobot&&resCam)
    {
        ui->colorConnected->setStyleSheet("QLabel#colorConnected { background-color :  green;}");
    }
    else if((resRobot&&!resCam)||(!resRobot&&resCam))
    {
        ui->colorConnected->setStyleSheet("QLabel#colorConnected { background-color :  orange;}");
    }
    else
    {
        ui->colorConnected->setStyleSheet("QLabel#colorConnected { background-color :  red;}");
    }
    //ui->webView->load(QUrl("http://192.168.1.106:8080/javascript_simple.html"));
}

void Interface::on_robotStop_clicked()
{
    Clientcont->stopConnectionRobot();
    //Clientcam->stopConnectionCamera();
    this->setcolorConnected("red");
}

void Interface::setcolorConnected(QString color)
{
    ui->colorConnected->setStyleSheet("QLabel#colorConnected { background-color :  "+color+";}");
}

Interface::~Interface()
{
    delete ui;
}

void Interface::on_actionPort_et_IP_triggered()
{
    Config* window= new Config();
    window->exec();
}



void Interface::majInterface(RobotInfo dataR, RobotInfo dataL)
{
    ui->batteryLevel->setValue((int)(dataL.getBatLevel()/2.55));
    ui->displaySpeed->display(5);

}
void Interface::setImage(QImage img)
{
     ui->frame->setPixmap(QPixmap::fromImage(img));
}

//************** CONTROLE CAMERA **************//

void Interface::on_cameraLeft_clicked()
{
    QPixmap mypix (":/image/image/arrowLeftPressed.png");
    ui->cameraLeft->setPixmap(mypix);
    /*req.setUrl(QUrl(QString("http://"+Clientcam->getIp()+":"+Clientcam->getPort()+"/?action=command&dest=0&plugin=0&id=10094852&group=1&value=200")));
    mgr.get(req);*/
    Qpressed=true;
    ControlCam();
}
void Interface::on_cameraLeft_released()
{
    QPixmap mypix (":/image/image/arrowLeft.png");
    ui->cameraLeft->setPixmap(mypix);
    Qpressed=false;
    ControlCam();
}
void Interface::on_cameraUp_released()
{
    QPixmap mypix (":/image/image/arrowUp.png");
    ui->cameraUp->setPixmap(mypix);
    Zpressed=false;
    ControlCam();
}
void Interface::on_cameraUp_clicked()
{
    QPixmap mypix (":/image/image/arrowUpPressed.png");
    ui->cameraUp->setPixmap(mypix);
   /* req.setUrl(QUrl(QString("http://"+Clientcam->getIp()+":"+Clientcam->getPort()+"/?action=command&dest=0&plugin=0&id=10094853&group=1&value=-200")));
    mgr.get(req);*/
    Zpressed=true;
    ControlCam();
}
void Interface::on_cameraDown_released()
{
    QPixmap mypix (":/image/image/arrowDown.png");
    ui->cameraDown->setPixmap(mypix);
    Spressed=false;
    ControlCam();
}
void Interface::on_cameraDown_clicked()
{
    QPixmap mypix (":/image/image/arrowDownPressed.png");
    ui->cameraDown->setPixmap(mypix);
    /*req.setUrl(QUrl(QString("http://"+Clientcam->getIp()+":"+Clientcam->getPort()+"/?action=command&dest=0&plugin=0&id=10094853&group=1&value=200")));
    mgr.get(req);*/
    Spressed=true;
    ControlCam();
}
void Interface::on_cameraRight_released()
{
    QPixmap mypix (":/image/image/arrowRight.png");
    ui->cameraRight->setPixmap(mypix);
    Dpressed=false;
    ControlCam();
}
void Interface::on_cameraRight_clicked()
{
    QPixmap mypix (":/image/image/arrowRightPressed.png");
    ui->cameraRight->setPixmap(mypix);
    /*req.setUrl(QUrl(QString("http://"+Clientcam->getIp()+":"+Clientcam->getPort()+"/?action=command&dest=0&plugin=0&id=10094852&group=1&value=-200")));
    mgr.get(req);*/
    Dpressed=true;
    ControlCam();
}


//************** CONTROLE ROBOT **************//

void Interface::on_robotRight_clicked()
{
    QPixmap mypix (":/image/image/arrowRightPressed.png");
    ui->robotRight->setPixmap(mypix);
    Lpressed=true;
    ControlDirection();
}

void Interface::on_robotRight_released()
{
    QPixmap mypix (":/image/image/arrowRight.png");
    ui->robotRight->setPixmap(mypix);
    Lpressed=false;
    ControlDirection();
}

void Interface::on_robotUp_clicked()
{
    QPixmap mypix (":/image/image/arrowUpPressed.png");
    ui->robotUp->setPixmap(mypix);
    Ipressed=true;
    ControlDirection();
}

void Interface::on_robotUp_released()
{
    QPixmap mypix (":/image/image/arrowUp.png");
    ui->robotUp->setPixmap(mypix);
    Ipressed=false;
    ControlDirection();
}

void Interface::on_robotDown_clicked()
{
    QPixmap mypix (":/image/image/arrowDownPressed.png");
    ui->robotDown->setPixmap(mypix);
    Kpressed=true;
    ControlDirection();
}

void Interface::on_robotDown_released()
{
    QPixmap mypix (":/image/image/arrowDown.png");
    ui->robotDown->setPixmap(mypix);
    Kpressed=false;
    ControlDirection();
}

void Interface::on_robotLeft_clicked()
{
    QPixmap mypix (":/image/image/arrowLeftPressed.png");
    ui->robotLeft->setPixmap(mypix);
    Jpressed=true;
    ControlDirection();
}

void Interface::on_robotLeft_released()
{
    QPixmap mypix (":/image/image/arrowLeft.png");
    ui->robotLeft->setPixmap(mypix);
    Jpressed=false;
    ControlDirection();
}

