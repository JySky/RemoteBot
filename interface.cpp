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
    camAuto=false;
    imgProc=false;
    bImshow=false;
    controllerOn=false;
    /*
     *
     *
     *
     *
     * AJOUTER FONCTION ENREGISTRER IMAGE
     *
     *
     * */

    QObject::connect(this, SIGNAL(robotDisconnect()),Clientcont, SLOT(socketDisconnected()), Qt::DirectConnection);
    QObject::connect(this, SIGNAL(camDisconnect()),Clientcam, SLOT(disconnect()), Qt::DirectConnection);
    QObject::connect(this, SIGNAL(startImshow()),Clientcam, SLOT(openImshow()), Qt::DirectConnection);
    QObject::connect(this, SIGNAL(stopImshow()),Clientcam, SLOT(closeImshow()), Qt::DirectConnection);
    QObject::connect(this, SIGNAL(startImgProcessing()),Clientcam, SLOT(startImgProcess()), Qt::DirectConnection);
    QObject::connect(this, SIGNAL(stopImgProcessing()),Clientcam, SLOT(stopImgProcess()), Qt::DirectConnection);
}

Interface::~Interface()
{
    delete ui;
}

//********************************** KEY EVENTS **********************************//

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

//********************************** SLOTS **********************************//

void Interface::setFrame(QImage fr)
{
    setImage(fr);
}

void Interface::camDisconnected()
{
    camconnected=false;
    std::string s ="Disconnected from "+Clientcam->getIp().toStdString()+"/"+(QString::number(Clientcam->getPort())).toStdString();
    const char* msg=s.c_str();
    QMessageBox::information(this,tr("Camera"),tr(msg));
    majConnectedState();
}

void Interface::robotDisconnected()
{

    robotconnected=false;
    std::string s ="Disconnected from "+Clientcont->getIp().toStdString()+"/"+(QString::number(Clientcont->getPort())).toStdString();
    const char* msg=s.c_str();
    QMessageBox::information(this,tr("Robot"),tr(msg));
    majConnectedState();
}

void Interface::robotConnected()
{
    robotconnected=true;
    majConnectedState();
}

void Interface::camConnected()
{
    camconnected=true;
    majConnectedState();
}

void Interface::camStreamState()
{

}

void Interface::majConnectedState()
{

    if(robotconnected&&camconnected)
    {
        QPixmap mypix (":/image/image/connected.png");
        ui->colorConnected->setPixmap(mypix);
    }
    else if((robotconnected&&!camconnected)||(!robotconnected&&camconnected))
    {

        QPixmap mypix (":/image/image/connectwarning.png");
        ui->colorConnected->setPixmap(mypix);
        if(!camconnected)
        {
            ui->frame->setPixmap(QPixmap());
            ui->frame->setText("NO SIGNAL");
            this->repaint();
        }
    }
    else
    {
        ui->frame->setPixmap(QPixmap());
        ui->frame->setText("NO SIGNAL");
        this->repaint();
        QPixmap mypix (":/image/image/disconnected.png");
        ui->colorConnected->setPixmap(mypix);
    }
}

void Interface::robotNotConnected()
{
    robotconnected=false;
    std::string s ="Not connected to"+Clientcont->getIp().toStdString()+"/"+(QString::number(Clientcont->getPort())).toStdString();
    const char* msg=s.c_str();
    QMessageBox::critical(this,tr("Robot Error"),tr(msg));
    majConnectedState();
}
void Interface::camNotConnected()
{
    camconnected=false;
    std::string s ="Not connected to"+Clientcam->getIp().toStdString()+"/"+(QString::number(Clientcam->getPort())).toStdString();
    const char* msg=s.c_str();
    QMessageBox::critical(this,tr("Camera Error"),tr(msg));
    majConnectedState();
}

void Interface::on_robotStart_clicked()
{
    Clientcont->start();
    Clientcam->start();
    majConnectedState();
}

void Interface::on_robotStop_clicked()
{
    emit robotDisconnect();
    emit camDisconnect();
    majConnectedState();
}

void Interface::on_actionPort_et_IP_triggered()
{
    Config* window= new Config();
    window->exec();
}

//********************************** SET GET **********************************//

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

void Interface::setBatLevel(int lvl)
{
    if(lvl>140)
    {
        ui->batteryLevel->setValue(100);
        QPixmap mypix (":/image/image/batteryCharging.png");
        ui->batteryLabel->setPixmap(mypix);
    }else if(lvl<40)
    {
        QPixmap mypix (":/image/image/batteryLow.png");
        ui->batteryLabel->setPixmap(mypix);
        ui->batteryLevel->setValue((lvl*100)/130);
    }
    else
    {
        QPixmap mypix (":/image/image/battery.png");
        ui->batteryLabel->setPixmap(mypix);
        ui->batteryLevel->setValue((lvl*100)/130);
    }
}

void Interface::setImage(QImage img)
{
     ui->frame->setPixmap(QPixmap::fromImage(img));
}

void Interface::setImage(QString img)
{
     ui->frame->setPixmap(img);
}

void Interface::setVitLeft(int lvl)
{
    /*if((lvl>40) && (lvl<130))
    {
        ui->pbSpeedLF->setValue(25);
    }else if((lvl<(-90)) && (lvl>(-130)))
    {
        ui->pbSpeedLF->setValue(80);
    }else
    {
        ui->pbSpeedLF->setValue(0);
    }

    if((lvl<(-130)) && (lvl>(-180)))
    {
        ui->pbSpeedLR->setValue(25);
    }else if((lvl<(-300)) && (lvl>(-390)))
    {
        ui->pbSpeedLR->setValue(80);
    }
    else
    {
        ui->pbSpeedLR->setValue(0);
    }*/
}

void Interface::setVitRight(int lvl)
{
    /*if((lvl>40) && (lvl<130))
    {
        ui->pbSpeedRF->setValue(25);
    }else if((lvl<(-90)) && (lvl>(-130)))
    {
        ui->pbSpeedRF->setValue(80);
    }else
    {
        ui->pbSpeedRF->setValue(0);
    }

    if((lvl<(-130)) && (lvl>(-180)))
    {
        ui->pbSpeedRR->setValue(25);
    }else if((lvl<(-300)) && (lvl>(-390)))
    {
        ui->pbSpeedRR->setValue(80);
    }
    else
    {
        ui->pbSpeedRR->setValue(0);
    }*/
}

void Interface::setIR1(int lvl)
{
    ui->displayIR1->setNum(lvl);
}

void Interface::setIR2(int lvl)
{
    ui->displayIR2->setNum(lvl);
}

void Interface::setIR3(int lvl)
{
    ui->displayIR3->setNum(lvl);
}

void Interface::setIR4(int lvl)
{
    ui->displayIR4->setNum(lvl);
}

void Interface::setVersion(int vers)
{
    ui->displayVersion->setNum(vers);
}

void Interface::setCurrent(int curr)
{
    ui->displayCurrent->setNum(curr);
}

void Interface::setAngle(int ang)
{
    ui->displayCamAngle->setNum(ang);
}

int Interface::getSliderCam()
{
    return ui->sliderSpeedCam->value();
}


//********************************** CAMERA CONTROL **********************************//


void Interface::ControlCam()
{
    if(Zpressed && !Qpressed && !Spressed && !Dpressed)
    { // haut
        Clientcam->moveCam(1);
        QPixmap mypix (":/image/image/arrowUpPressed.png");
        ui->cameraUp->setPixmap(mypix);
        QPixmap mypix1 (":/image/image/arrowLeft.png");
        ui->cameraLeft->setPixmap(mypix1);
        QPixmap mypix2 (":/image/image/arrowRight.png");
        ui->cameraRight->setPixmap(mypix2);
        QPixmap mypix3 (":/image/image/arrowDown.png");
        ui->cameraDown->setPixmap(mypix3); 
    }
    else if(!Zpressed && !Qpressed && Spressed && !Dpressed)
    { //bas
         Clientcam->moveCam(2);
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
        Clientcam->moveCam(3);
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
        Clientcam->moveCam(4);
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
        Clientcam->moveCam(6);
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
        Clientcam->moveCam(5);
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
        Clientcam->moveCam(7);
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
        Clientcam->moveCam(8);
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

void Interface::on_dotCamera_clicked()
{
    Clientcam->moveCam(9);
    QPixmap mypix (":/image/image/arrowDotCameraPressed.png");
    ui->dotCamera->setPixmap(mypix);
}

void Interface::on_dotCamera_released()
{
    Clientcam->moveCam(9);
    QPixmap mypix (":/image/image/arrowDotCamera.png");
    ui->dotCamera->setPixmap(mypix);
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

//********************************** ROBOT CONTROL **********************************//

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
        Clientcont->setLeftSpeedFlag(0);
        Clientcont->setRightSpeedFlag(1);
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
        Clientcont->setLeftSpeedFlag(1);
        Clientcont->setRightSpeedFlag(0);
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
        Clientcont->setLeftSpeed((int)((ui->displaySpeedSet->text().toInt())*0.7));
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
        Clientcont->setRightSpeed((int)((ui->displaySpeedSet->text().toInt())*0.7));
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
        Clientcont->setLeftSpeedFlag(0);
        Clientcont->setRightSpeedFlag(0);
        Clientcont->setLeftSpeed((int)((ui->displaySpeedSet->text().toInt())*0.7));
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
        Clientcont->setLeftSpeedFlag(0);
        Clientcont->setRightSpeedFlag(0);
        Clientcont->setLeftSpeed((int)((ui->displaySpeedSet->text().toInt())*2.4));
        Clientcont->setRightSpeed((int)((ui->displaySpeedSet->text().toInt())*0.7));
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



//********************************** FRAME ACTION **********************************//

void Interface::on_actionQuitter_triggered()
{
    this->close();
}

void Interface::on_actionActiver_Manette_changed()
{
    if(imgProc)
    {
        controllerOn=false;
    }else
    {
        controllerOn=true;
    }
}

void Interface::on_actionCamera_Automatique_changed()
{
    if(imgProc)
    {
        camAuto=false;
    }else
    {
        camAuto=true;
    }
    Clientcam->setCamAuto(camAuto);
}

void Interface::on_actionA_propos_triggered()
{
    About* window= new About();
    window->exec();
}

void Interface::on_actionTutoriel_triggered()
{
    tutorial* window= new tutorial();
    window->exec();
}

void Interface::on_actionActiver_Traitement_Image_changed()
{
    if(imgProc)
        imgProc=false;
    else if(!imgProc)
        imgProc=true;

    if(imgProc)
    {
        emit startImgProcessing();
    }
    else
    {
        emit stopImgProcessing();
    }
}

void Interface::on_actionImshow_OpenCV_changed()
{
    if(bImshow)
        bImshow=false;
    else if(!bImshow)
        bImshow=true;

    if(bImshow)
    {
        emit startImshow();
    }
    else
    {
        emit stopImshow();
    }
}

