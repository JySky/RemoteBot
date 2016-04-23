#include "interface.h"
#include "ui_interface.h"
#include <QKeyEvent>

//********************************** CONSTRUCTOR **********************************//

Interface::Interface(QWidget *parent) : QMainWindow(parent), ui(new Ui::Interface)
{
    ui->setupUi(this);
    Clientcont = ClientControl::getInstance((this->parent()),this);
    Clientcam = ClientCamera::getInstance((this->parent()),this);
    initinterface();
    initConnect();
    QObject::connect(ui->menuA_propos, SIGNAL(aboutToShow()),this, SLOT(a_propos()), Qt::QueuedConnection);
}

Interface::~Interface()
{
    delete ui;
}

//********************************** INIT **********************************//

void Interface::initinterface()
{
    mutex.lock();
        IR1=0;
        oldIR1=0;
        IR2=0;
        oldIR2=0;
        IR3=0;
        oldIR3=0;
        IR4=0;
        oldIR4=0;
        odoL=0;
        odoR=0;
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
        /*ui->displayIR1->setNum(0);
        ui->displayIR2->setNum(0);
        ui->displayIR3->setNum(0);
        ui->displayIR4->setNum(0);*/
        ui->displayVersion->setNum(0);
        ui->displayCurrent->setNum(0);
        ui->displaySpeed->setNum(0);
        ui->batteryLevel->setValue(0);
        ui->frame->setPixmap(QPixmap());
        ui->frame->setText("NO SIGNAL");
        ui->displayIR1->setStyleSheet("QLabel {background-color : rgb(3,255,45); }");
        ui->displayIR2->setStyleSheet("QLabel {background-color : rgb(3,255,45); }");
        ui->displayIR3->setStyleSheet("QLabel {background-color : rgb(3,255,45); }");
        ui->displayIR4->setStyleSheet("QLabel {background-color : rgb(3,255,45); }");
        this->repaint();
        robotconnected=false;
        camconnected=false;
    mutex.unlock();
}

void Interface::initConnect()
{
    QObject::connect(this, SIGNAL(robotDisconnect()),Clientcont, SLOT(socketDisconnected()), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(camDisconnect()),Clientcam, SLOT(disconnect()), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(startImshow()),Clientcam, SLOT(openImshow()), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(stopImshow()),Clientcam, SLOT(closeImshow()), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(startImgProcessing()),Clientcam, SLOT(startImgProcess()), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(stopImgProcessing()),Clientcam, SLOT(stopImgProcess()), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(RightSpeed(unsigned char)),Clientcont, SLOT(setRightSpeed(unsigned char)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(LeftSpeed(unsigned char)),Clientcont, SLOT(setLeftSpeed(unsigned char)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(RightSpeedFlag(unsigned char)),Clientcont, SLOT(setRightSpeedFlag(unsigned char)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(LeftSpeedFlag(unsigned char)),Clientcont, SLOT(setLeftSpeedFlag(unsigned char)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(robotConnect()),Clientcont, SLOT(robotStart()), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(camConnect()),Clientcam, SLOT(cameraStart()), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(sendSliderCamValue(int)),Clientcam, SLOT(receiveSliderCamValue(int)), Qt::QueuedConnection);

    QObject::connect(this, SIGNAL(robotFrontCollisionOn()),Clientcont, SLOT(frontCollisionOn()), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(robotFrontCollisionOff()),Clientcont, SLOT(frontCollisionOff()), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(robotRearCollisionOn()),Clientcont, SLOT(rearCollisionOn()), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(robotRearCollisionOff()),Clientcont, SLOT(rearCollisionOff()), Qt::QueuedConnection);

    QObject::connect(this, SIGNAL(moveCamera(int)),Clientcam, SLOT(moveCam(int)), Qt::QueuedConnection);
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
    majConnectedState();
}

void Interface::robotDisconnected()
{

    robotconnected=false;
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

void Interface::majConnectedState()
{
    mutex.lock();
        if(robotconnected&&camconnected)
        {
            QPixmap mypix (":/image/image/connected.png");
            ui->colorConnected->setPixmap(mypix);
        }
        else if(robotconnected&&!camconnected)
        {
            QPixmap mypix(":/image/image/connectwarningcamera.png");
            ui->colorConnected->setPixmap(mypix);
            if(!camconnected)
            {
                ui->frame->setPixmap(QPixmap());
                ui->frame->setText("NO SIGNAL");
                this->repaint();
            }
        }
        else if(!robotconnected&&camconnected)
        {

            QPixmap mypix (":/image/image/connectwarningrobot.png");
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
    mutex.unlock();
}

void Interface::robotNotConnected()
{
    robotconnected=false;
    std::string s ="Not connected to host, please check the IP and port.";
    const char* msg=s.c_str();
    QMessageBox::warning(this,tr("Robot Connection Failed"),tr(msg));
    majConnectedState();
    mutex.lock();
        Clientcont = ClientControl::getInstance((this->parent()),this);
        Clientcam = ClientCamera::getInstance((this->parent()),this);
    mutex.unlock();
    initConnect();
}

void Interface::camNotConnected()
{
    camconnected=false;

    std::string s ="Not connected to host, please check the IP and port.";
    const char* msg=s.c_str();
    QMessageBox::warning(this,tr("Camera Connection Failed"),tr(msg));

    majConnectedState();
    mutex.lock();
        Clientcont = ClientControl::getInstance((this->parent()),this);
        Clientcam = ClientCamera::getInstance((this->parent()),this);
    mutex.unlock();
    initConnect();
}

void Interface::on_robotStart_clicked()
{
    mutex.lock();
        Clientcont = ClientControl::getInstance((this->parent()),this);
        Clientcam = ClientCamera::getInstance((this->parent()),this);
    mutex.unlock();

    initConnect();
    emit robotConnect();
    emit camConnect();
    majConnectedState();
}

void Interface::on_robotStop_clicked()
{
    emit robotDisconnect();
    emit camDisconnect();
    initinterface();
    majConnectedState();
    mutex.lock();
        Clientcont = ClientControl::getInstance((this->parent()),this);
        Clientcam = ClientCamera::getInstance((this->parent()),this);
    mutex.unlock();
    initConnect();
}

void Interface::on_actionPort_et_IP_triggered()
{
    mutex.lock();
        Clientcont = ClientControl::getInstance((this->parent()),this);
        Clientcam = ClientCamera::getInstance((this->parent()),this);
    mutex.unlock();
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
}

void Interface::setBatLevel(int lvl)
{
    mutex.lock();
    if(lvl>140)
    {
        ui->batteryLevel->setValue(100);
        QPixmap mypix (":/image/image/batteryCharging.png");
        ui->batteryLabel->setPixmap(mypix);
    }else if(lvl<40)
    {
        QPixmap mypix (":/image/image/batteryLow.png");
        ui->batteryLabel->setPixmap(mypix);
        ui->batteryLevel->setValue((lvl*100)/128);
    }
    else
    {
        QPixmap mypix (":/image/image/battery.png");
        ui->batteryLabel->setPixmap(mypix);
        ui->batteryLevel->setValue((lvl*100)/130);
    }
    mutex.unlock();
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
    mutex.lock();
        if(lvl>0)
        {
            ui->pbSpeedLF->setValue(lvl);
        }else if(lvl<0)
        {
            ui->pbSpeedLR->setValue((int)qFloor(lvl));
        }else
        {
            ui->pbSpeedRR->setValue(0);
            ui->pbSpeedRF->setValue(0);
        }
        speedL=(int)qFloor(lvl);
        setVitesseGlobal();
    mutex.unlock();
}

void Interface::setVitRight(int lvl)
{
    mutex.lock();
        if(lvl>0)
        {
            ui->pbSpeedRF->setValue(lvl);
        }else if(lvl<0)
        {
            ui->pbSpeedRR->setValue((int)qFloor(lvl));
        }else
        {
            ui->pbSpeedRR->setValue(0);
            ui->pbSpeedRF->setValue(0);
        }
        speedR=(int)qFloor(lvl);
        setVitesseGlobal();
    mutex.unlock();
}

void Interface::setVitesseGlobal()
{
    mutex.lock();
        int vitleft, vitright,rpmPerTics;
        //2048 / wheel turn     1 wheel turn =46.5cm   156rpm->240tics/set  1r->2048 tics
        int res;
        vitleft=speedL;
        vitright=speedR;
        res=(int)((vitright+vitleft)/2);
        rpmPerTics=(int)((res*156)/240);
        res=(int)((rpmPerTics*0.5)/60);//en m/s
        ui->displaySpeed->setNum(res);
    mutex.unlock();
}

int Interface::computeDistance(float lvl, float oldLvl)
{
    int res=255;
    int lvltmp=lvl*(255/3.3);
    if(!lvltmp==255)
    {
        if(lvl>oldLvl)
        {
            res=(int)((qPow(1.9, -4)*qPow(lvl, 2))+(qPow(4.6, -2)*lvl)+(3.27));
        }
        else if (lvl<oldLvl)
        {
            res=(int)(0.183*lvl);
        }
    }
    return res;
}

void Interface::setIR1(int lvl)//Avant-Droit
{
    mutex.lock();
        /*IR1=lvl;
        ui->displayIR1->setNum(IR1);
        oldIR1=IR1;*/

        IR1=lvl;
        if(IR1<oldIR1)//rapprochement
        {
            if(IR1>156)//30cm = trop prés
            {
                emit robotFrontCollisionOn();
                ui->displayIR1->setStyleSheet("QLabel {background-color : rgb(255,40,40); }");
            }
            else
            {
                emit robotFrontCollisionOff();
                ui->displayIR1->setStyleSheet("QLabel {background-color : rgb(3,255,45); }");
            }
        }
        else//éloignement ou autres
        {
            ui->displayIR1->setStyleSheet("QLabel {background-color : rgb(3,255,45); }");
        }
        oldIR1=IR1;
    mutex.unlock();
}

void Interface::setIR2(int lvl)//Arrière-Gauche
{
    mutex.lock();
        /*IR2=lvl;
        ui->displayIR2->setNum(IR2);//(unsigned char)(lvl*(3.3/255)));
        oldIR2=IR2;*/

        IR2=lvl;
        if(IR2<oldIR2)//rapprochement
        {
            if(IR2>156)//30cm = trop prés
            {
                emit robotFrontCollisionOn();
                ui->displayIR2->setStyleSheet("QLabel {background-color : rgb(255,40,40); }");//>setNum(IR4);
            }
            else
            {
                emit robotFrontCollisionOff();
                ui->displayIR2->setStyleSheet("QLabel {background-color : rgb(3,255,45); }");//>setNum(IR4);
            }
        }
        else//éloignement ou autres
        {
            ui->displayIR2->setStyleSheet("QLabel {background-color : rgb(3,255,45); }");//>setNum(IR4);
        }
        oldIR2=IR2;
    mutex.unlock();
}

void Interface::setIR3(int lvl)//Arrière-Droit
{
    mutex.lock();
        /*IR3=lvl;
        ui->displayIR3->setNum(IR3);//(unsigned char)(lvl*(3.3/255)));
        oldIR3=IR3;*/

        IR3=lvl;
        if(IR3<oldIR3)//rapprochement
        {
            if(IR3>156)//30cm = trop prés
            {
                emit robotFrontCollisionOn();
                ui->displayIR3->setStyleSheet("QLabel {background-color : rgb(255,40,40); }");//>setNum(IR4);
            }
            else
            {
                emit robotFrontCollisionOff();
                ui->displayIR3->setStyleSheet("QLabel {background-color : rgb(3,255,45); }");//>setNum(IR4);
            }
        }
        else//éloignement ou autres
        {
            ui->displayIR3->setStyleSheet("QLabel {background-color : rgb(3,255,45); }");//>setNum(IR4);
        }
        oldIR3=IR3;
    mutex.unlock();
}

void Interface::setIR4(int lvl) //Avant-Gauche
{
    mutex.lock();
        IR4=lvl;
        if(IR4<oldIR4)//rapprochement
        {
            if(IR4>156)//30cm = trop prés
            {
                emit robotFrontCollisionOn();
                ui->displayIR4->setStyleSheet("QLabel {background-color : rgb(255,40,40); }");//>setNum(IR4);
            }
            else
            {
                emit robotFrontCollisionOff();
                ui->displayIR4->setStyleSheet("QLabel {background-color : rgb(3,255,45); }");//>setNum(IR4);
            }
        }
        else//éloignement ou autres
        {
            ui->displayIR4->setStyleSheet("QLabel {background-color : rgb(3,255,45); }");//>setNum(IR4);
        }
        oldIR4=IR4;
    mutex.unlock();
}

void Interface::setVersion(int vers)
{
    mutex.lock();
        ui->displayVersion->setNum(vers);
    mutex.unlock();
}

void Interface::setCurrent(int curr)
{
    mutex.lock();
        ui->displayCurrent->setNum((int)(curr));//*0.194-37.5));
    mutex.unlock();
}

void Interface::setOdoL(long odo)
{
    mutex.lock();
        odoL=odo;
    mutex.unlock();
}

void Interface::setOdoR(long odo)
{
    mutex.lock();
        odoR=odo;
    mutex.unlock();
}

void Interface::getSliderCamValue()
{
    mutex.lock();
        emit sendSliderCamValue(ui->sliderSpeedCam->value());
    mutex.unlock();
}

//********************************** CAMERA CONTROL **********************************//


void Interface::ControlCam()
{
    //mutex.lock();
    if(Zpressed && !Qpressed && !Spressed && !Dpressed)
    { // haut
        emit moveCamera(1);
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
        emit moveCamera(2);
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
        emit moveCamera(3);
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
        emit moveCamera(4);
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
        emit moveCamera(6);
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
        emit moveCamera(5);
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
        emit moveCamera(7);
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
        emit moveCamera(8);
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
    //mutex.unlock();
}

void Interface::on_dotCamera_clicked()
{
    emit moveCamera(9);
    QPixmap mypix (":/image/image/arrowDotCameraPressed.png");
    ui->dotCamera->setPixmap(mypix);
}

void Interface::on_dotCamera_released()
{
    //emit moveCamera(9);
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
        emit LeftSpeedFlag(1); // mise en route gauche
        emit RightSpeedFlag(1); // mise en route droit
        emit LeftSpeed((int)((ui->displaySpeedSet->text().toInt())*2.4)); // vitesse gauche
        emit RightSpeed((int)((ui->displaySpeedSet->text().toInt())*2.4)); // vitesse droit
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

        emit LeftSpeedFlag(0);
        emit RightSpeedFlag(0);
        emit LeftSpeed((int)((ui->displaySpeedSet->text().toInt())*2.4));
        emit RightSpeed((int)((ui->displaySpeedSet->text().toInt())*2.4));
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

        emit LeftSpeedFlag(0);
        emit RightSpeedFlag(1);
        emit LeftSpeed((int)((ui->displaySpeedSet->text().toInt())*2.4));
        emit RightSpeed((int)((ui->displaySpeedSet->text().toInt())*2.4));
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
        emit LeftSpeedFlag(1);
        emit RightSpeedFlag(0);
        emit LeftSpeed((int)((ui->displaySpeedSet->text().toInt())*2.4));
        emit RightSpeed((int)((ui->displaySpeedSet->text().toInt())*2.4));
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

        emit LeftSpeedFlag(1);
        emit RightSpeedFlag(1);
        emit LeftSpeed((int)((ui->displaySpeedSet->text().toInt())*0.7));
        emit RightSpeed((int)((ui->displaySpeedSet->text().toInt())*2.4));
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

        emit LeftSpeedFlag(1);
        emit RightSpeedFlag(1);
        emit LeftSpeed((int)((ui->displaySpeedSet->text().toInt())*2.4));
        emit RightSpeed((int)((ui->displaySpeedSet->text().toInt())*0.7));
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

        emit LeftSpeedFlag(0);
        emit RightSpeedFlag(0);
        emit LeftSpeed((int)((ui->displaySpeedSet->text().toInt())*1.4));
        emit RightSpeed((int)((ui->displaySpeedSet->text().toInt())*2.4));
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

        emit LeftSpeedFlag(0);
        emit RightSpeedFlag(0);
        emit LeftSpeed((int)((ui->displaySpeedSet->text().toInt())*2.4));
        emit RightSpeed((int)((ui->displaySpeedSet->text().toInt())*1.4));
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
        emit LeftSpeedFlag(1);
        emit RightSpeedFlag(1);
        emit LeftSpeed((int)((ui->displaySpeedSet->text().toInt())*0));
        emit RightSpeed((int)((ui->displaySpeedSet->text().toInt())*0));
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


void Interface::a_propos()
{
    About* window= new About();
    window->exec();
}

void Interface::receiveIPCam(QString ipc)
{
    emit setConfigIPCam(ipc);
}

void Interface::receivePortCam(int portc)
{
    emit setConfigPortCam(portc);
}

void Interface::receiveIPRobot(QString ipr)
{
    emit setConfigIPRobot(ipr);
}

void Interface::receivePortRobot(int portr)
{
    emit setConfigPortRobot(portr);
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

