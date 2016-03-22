#include "interface.h"
#include "ui_interface.h"
#include <QKeyEvent>


Interface::Interface(QWidget *parent) : QMainWindow(parent), ui(new Ui::Interface)
{
    ui->setupUi(this);
    Clientcont = ClientControl::getInstance(this);
    Clientcam = ClientCamera::getInstance(this);
    Ipressed=false;
    Jpressed=false;
    Kpressed=false;
    Lpressed=false;
    Zpressed=false;
    Qpressed=false;
    Spressed=false;
    Dpressed=false;
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
            break;

        case Qt::Key_S:
                Spressed=true;
            break;

        case Qt::Key_Q:
                Qpressed=true;
            break;

        case Qt::Key_D:
                Dpressed=true;
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

    if(resRobot&&resCam)
    {
        QPixmap mypix (":/image/image/connected.png");
        ui->colorConnected->setPixmap(mypix);
    }
    else if((resRobot&&!resCam)||(!resRobot&&resCam))
    {

        QPixmap mypix (":/image/image/connectwarning.png");
        ui->colorConnected->setPixmap(mypix);
    }
    else
    {
        QPixmap mypix (":/image/image/disconnected.png");
        ui->colorConnected->setPixmap(mypix);
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
    if(color=="green")
    {
        QPixmap mypix (":/image/image/connected.png");
        ui->colorConnected->setPixmap(mypix);
    }else if (color=="orange")
    {
        QPixmap mypix (":/image/image/connectwarning.png");
        ui->colorConnected->setPixmap(mypix);
    }else if (color=="red")
    {
        QPixmap mypix (":/image/image/disconnected.png");
        ui->colorConnected->setPixmap(mypix);
    }
    //ui->colorConnected->setStyleSheet("QLabel#colorConnected { background-color :  "+color+";}");
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
   /* if(dataR.SpeedFront>=0)
    {
        ui->pbSpeedRF->setValue(dataR.SpeedFront);
    }
    else
    {
         ui->pbSpeedRR->setValue(dataR.SpeedFront);
    }

    if(dataL.SpeedFront>=0)
    {

        ui->pbSpeedLF->setValue(dataR.SpeedFront);
    }
    else
    {
         ui->pbSpeedLR->setValue(dataR.SpeedFront);
    }*/
   // ui->displaySpeed->display(5);
}

void Interface::setImage(QImage img)
{
     ui->frame->setPixmap(QPixmap::fromImage(img));
}

//************** CONTROLE CAMERA **************//
void Interface::ControlCam()
{
    if(Zpressed && !Qpressed && !Spressed && !Dpressed)
    { // haut
        QPixmap mypix (":/image/image/arrowUpPressed.png");
        ui->cameraUp->setPixmap(mypix);
        QPixmap mypix1 (":/image/image/arrowLeft.png");
        ui->cameraLeft->setPixmap(mypix1);
        QPixmap mypix2 (":/image/image/arrowRight.png");
        ui->cameraRight->setPixmap(mypix2);
        QPixmap mypix3 (":/image/image/arrowDown.png");
        ui->cameraDown->setPixmap(mypix3);
        req.setUrl((QUrl(QString("http://"+Clientcam->getIp()+":"+Clientcam->getPort()+"/?action=command&dest=0&plugin=0&id=10094853&group=1&value=-200"))));
        mgr.get(req);
    }
    else if(!Zpressed && !Qpressed && Spressed && !Dpressed)
    { //bas
        req.setUrl((QUrl(QString("http://"+Clientcam->getIp()+":"+Clientcam->getPort()+"/?action=command&dest=0&plugin=0&id=10094853&group=1&value=200"))));
        mgr.get(req);
        QPixmap mypix (":/image/image/arrowUp.png");
        ui->cameraUp->setPixmap(mypix);
        QPixmap mypix1 (":/image/image/arrowLeft.png");
        ui->cameraLeft->setPixmap(mypix1);
        QPixmap mypix2 (":/image/image/arrowRight.png");
        ui->cameraRight->setPixmap(mypix2);
        QPixmap mypix3 (":/image/image/arrowDownPressed.png");
        ui->cameraDown->setPixmap(mypix3);
    }
    else if(!Zpressed && Qpressed && !Spressed && !Dpressed)
    { // gauche
        req.setUrl((QUrl(QString("http://"+Clientcam->getIp()+":"+Clientcam->getPort()+"/?action=command&dest=0&plugin=0&id=10094852&group=1&value=200"))));
        mgr.get(req);
        QPixmap mypix (":/image/image/arrowUp.png");
        ui->cameraUp->setPixmap(mypix);
        QPixmap mypix1 (":/image/image/arrowLeftPressed.png");
        ui->cameraLeft->setPixmap(mypix1);
        QPixmap mypix2 (":/image/image/arrowRight.png");
        ui->cameraRight->setPixmap(mypix2);
        QPixmap mypix3 (":/image/image/arrowDown.png");
        ui->cameraDown->setPixmap(mypix3);
    }
    else if(!Zpressed && !Qpressed && !Spressed && Dpressed)
    { // droite
        req.setUrl((QUrl(QString("http://"+Clientcam->getIp()+":"+Clientcam->getPort()+"/?action=command&dest=0&plugin=0&id=10094852&group=1&value=-200"))));
        mgr.get(req);
        QPixmap mypix (":/image/image/arrowUp.png");
        ui->cameraUp->setPixmap(mypix);
        QPixmap mypix1 (":/image/image/arrowLeft.png");
        ui->cameraLeft->setPixmap(mypix1);
        QPixmap mypix2 (":/image/image/arrowRightPressed.png");
        ui->cameraRight->setPixmap(mypix2);
        QPixmap mypix3 (":/image/image/arrowDown.png");
        ui->cameraDown->setPixmap(mypix3);
    }
    else if(Zpressed && Qpressed && !Spressed && !Dpressed)
    { // haut gauche
        QPixmap mypix (":/image/image/arrowUpPressed.png");
        ui->cameraUp->setPixmap(mypix);
        QPixmap mypix1 (":/image/image/arrowLeftPressed.png");
        ui->cameraLeft->setPixmap(mypix1);
        QPixmap mypix2 (":/image/image/arrowRight.png");
        ui->cameraRight->setPixmap(mypix2);
        QPixmap mypix3 (":/image/image/arrowDown.png");
        ui->cameraDown->setPixmap(mypix3);
    }
    else if(Zpressed && !Qpressed && !Spressed && Dpressed)
    { // haut droite
        QPixmap mypix (":/image/image/arrowUpPressed.png");
        ui->cameraUp->setPixmap(mypix);
        QPixmap mypix1 (":/image/image/arrowLeft.png");
        ui->cameraLeft->setPixmap(mypix1);
        QPixmap mypix2 (":/image/image/arrowRightPressed.png");
        ui->cameraRight->setPixmap(mypix2);
        QPixmap mypix3 (":/image/image/arrowDown.png");
        ui->cameraDown->setPixmap(mypix3);
    }
    else if(!Zpressed && Qpressed && Spressed && !Dpressed)
    { // arrière gauche
        QPixmap mypix (":/image/image/arrowUp.png");
        ui->cameraUp->setPixmap(mypix);
        QPixmap mypix1 (":/image/image/arrowLeftPressed.png");
        ui->cameraLeft->setPixmap(mypix1);
        QPixmap mypix2 (":/image/image/arrowRight.png");
        ui->cameraRight->setPixmap(mypix2);
        QPixmap mypix3 (":/image/image/arrowDownPressed.png");
        ui->cameraDown->setPixmap(mypix3);
    }
    else if(!Zpressed && !Qpressed && Spressed && Dpressed)
    { // arrière droit
        QPixmap mypix (":/image/image/arrowUp.png");
        ui->cameraUp->setPixmap(mypix);
        QPixmap mypix1 (":/image/image/arrowLeft.png");
        ui->cameraLeft->setPixmap(mypix1);
        QPixmap mypix2 (":/image/image/arrowRightPressed.png");
        ui->cameraRight->setPixmap(mypix2);
        QPixmap mypix3 (":/image/image/arrowDownPressed.png");
        ui->cameraDown->setPixmap(mypix3);
    }
    else if((!Zpressed && !Qpressed && !Spressed && !Dpressed)||(Zpressed && !Qpressed && Spressed && !Dpressed)||(!Zpressed && Qpressed && !Spressed && Dpressed))
    { // stop
        QPixmap mypix (":/image/image/arrowUp.png");
        ui->cameraUp->setPixmap(mypix);
        QPixmap mypix1 (":/image/image/arrowLeft.png");
        ui->cameraLeft->setPixmap(mypix1);
        QPixmap mypix2 (":/image/image/arrowRight.png");
        ui->cameraRight->setPixmap(mypix2);
        QPixmap mypix3 (":/image/image/arrowDown.png");
        ui->cameraDown->setPixmap(mypix3);
    }
}

void Interface::on_cameraLeft_clicked()
{
    Qpressed=true;
    ControlCam();
}

void Interface::on_cameraLeft_released()
{
    Qpressed=false;
    ControlCam();
}

void Interface::on_cameraUp_released()
{
    Zpressed=false;
    ControlCam();
}

void Interface::on_cameraUp_clicked()
{
    Zpressed=true;
    ControlCam();
}

void Interface::on_cameraDown_released()
{
    Spressed=false;
    ControlCam();
}

void Interface::on_cameraDown_clicked()
{
    Spressed=true;
    ControlCam();
}

void Interface::on_cameraRight_released()
{
    Dpressed=false;
    ControlCam();
}

void Interface::on_cameraRight_clicked()
{
    Dpressed=true;
    ControlCam();
}


//************** CONTROLE ROBOT **************//
void Interface::ControlDirection()
{
    if(Ipressed && !Jpressed && !Kpressed && !Lpressed)
    { // avant
        Clientcont->setLeftSpeedFlag(1); // mise en route gauche
        Clientcont->setRightSpeedFlag(1); // mise en route droit
        Clientcont->setLeftSpeed((int)((ui->displaySpeedSet->text().toInt())*2.4)); // vitesse gauche
        Clientcont->setRightSpeed((int)((ui->displaySpeedSet->text().toInt())*2.4)); // vitesse droit
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
        Clientcont->setLeftSpeed((int)((ui->displaySpeedSet->text().toInt())*2.4));
        Clientcont->setRightSpeed((int)((ui->displaySpeedSet->text().toInt())*2.4));
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
        Clientcont->setLeftSpeed((int)((ui->displaySpeedSet->text().toInt())*2.4));
        Clientcont->setRightSpeed((int)((ui->displaySpeedSet->text().toInt())*2.4));
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
        Clientcont->setLeftSpeed((int)((ui->displaySpeedSet->text().toInt())*2.4));
        Clientcont->setRightSpeed((int)((ui->displaySpeedSet->text().toInt())*2.4));
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
        Clientcont->setLeftSpeed((int)((ui->displaySpeedSet->text().toInt())*1.4));
        Clientcont->setRightSpeed((int)((ui->displaySpeedSet->text().toInt())*2.4));
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
        Clientcont->setLeftSpeed((int)((ui->displaySpeedSet->text().toInt())*2.4));
        Clientcont->setRightSpeed((int)((ui->displaySpeedSet->text().toInt())*1.4));
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
        Clientcont->setLeftSpeed((int)((ui->displaySpeedSet->text().toInt())*1.4));
        Clientcont->setRightSpeed((int)((ui->displaySpeedSet->text().toInt())*2.4));
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
        Clientcont->setLeftSpeed((int)((ui->displaySpeedSet->text().toInt())*2.4));
        Clientcont->setRightSpeed((int)((ui->displaySpeedSet->text().toInt())*1.4));
        QPixmap mypix (":/image/image/arrowUp.png");
        ui->robotUp->setPixmap(mypix);
        QPixmap mypix1 (":/image/image/arrowLeft.png");
        ui->robotLeft->setPixmap(mypix1);
        QPixmap mypix2 (":/image/image/arrowRightPressed.png");
        ui->robotRight->setPixmap(mypix2);
        QPixmap mypix3 (":/image/image/arrowDownPressed.png");
        ui->robotDown->setPixmap(mypix3);
    }
    else if((!Kpressed && !Lpressed && !Ipressed && !Jpressed)||(Kpressed && !Lpressed && Ipressed && !Jpressed)||(!Kpressed && Lpressed && !Ipressed && Jpressed))
    { // stop
        Clientcont->setLeftSpeedFlag(1);
        Clientcont->setRightSpeedFlag(1);
        Clientcont->setLeftSpeed((int)((ui->displaySpeedSet->text().toInt())*0));
        Clientcont->setRightSpeed((int)((ui->displaySpeedSet->text().toInt())*0));
        QPixmap mypix (":/image/image/arrowUp.png");
        ui->robotUp->setPixmap(mypix);
        QPixmap mypix1 (":/image/image/arrowLeft.png");
        ui->robotLeft->setPixmap(mypix1);
        QPixmap mypix2 (":/image/image/arrowRight.png");
        ui->robotRight->setPixmap(mypix2);
        QPixmap mypix3 (":/image/image/arrowDown.png");
        ui->robotDown->setPixmap(mypix3);
    }
}

void Interface::on_robotRight_clicked()
{
    Lpressed=true;
    ControlDirection();
}

void Interface::on_robotRight_released()
{
    Lpressed=false;
    ControlDirection();
}

void Interface::on_robotUp_clicked()
{
    Ipressed=true;
    ControlDirection();
}

void Interface::on_robotUp_released()
{
    Ipressed=false;
    ControlDirection();
}

void Interface::on_robotDown_clicked()
{
    Kpressed=true;
    ControlDirection();
}

void Interface::on_robotDown_released()
{
    Kpressed=false;
    ControlDirection();
}

void Interface::on_robotLeft_clicked()
{
    Jpressed=true;
    ControlDirection();
}

void Interface::on_robotLeft_released()
{
    Jpressed=false;
    ControlDirection();
}

void Interface::on_actionQuitter_triggered()
{
    this->close();
}
