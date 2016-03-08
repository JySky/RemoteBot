#include "interface.h"
#include "ui_interface.h"
#include <QKeyEvent>

Interface::Interface(QWidget *parent) : QMainWindow(parent), ui(new Ui::Interface)
{
    ui->setupUi(this);
    Clientcont = ClientControl::getInstance(this);
    Clientcam = ClientCamera::getInstance(this);
}


void Interface::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Up:
                Clientcont->setLeftSpeedFlag(1);
                Clientcont->setRightSpeedFlag(1);
                Clientcont->setLeftSpeed((int)(ui->displaySpeedSet->value()*2.4));
                Clientcont->setRightSpeed((int)(ui->displaySpeedSet->value()*2.4));
            break;

        case Qt::Key_Down:
                Clientcont->setLeftSpeedFlag(1);
                Clientcont->setRightSpeedFlag(1);
                Clientcont->setLeftSpeed((int)(ui->displaySpeedSet->value()*2.4));
                Clientcont->setRightSpeed((int)(ui->displaySpeedSet->value()*2.4));
            break;

        case (Qt::Key_Left):
                Clientcont->setLeftSpeedFlag(1);
                Clientcont->setRightSpeedFlag(1);

            break;

        case (Qt::Key_Right):
                Clientcont->setLeftSpeedFlag(1);
                Clientcont->setRightSpeedFlag(1);
            break;

        case Qt::Key_Z:
            req.setUrl((QUrl(QString("http://"+Clientcam->getIp()+":"+Clientcam->getPort()+"/?action=command&dest=0&plugin=0&id=10094853&group=1&value=-200"))));
            mgr.get(req);
            break;

        case Qt::Key_S:
            req.setUrl((QUrl(QString("http://"+Clientcam->getIp()+":"+Clientcam->getPort()+"/?action=command&dest=0&plugin=0&id=10094853&group=1&value=200"))));
            mgr.get(req);
            break;

        case Qt::Key_Q:
            req.setUrl((QUrl(QString("http://"+Clientcam->getIp()+":"+Clientcam->getPort()+"/?action=command&dest=0&plugin=0&id=10094852&group=1&value=200"))));
            mgr.get(req);
            break;

        case Qt::Key_D:
            req.setUrl((QUrl(QString("http://"+Clientcam->getIp()+":"+Clientcam->getPort()+"/?action=command&dest=0&plugin=0&id=10094852&group=1&value=-200"))));
            mgr.get(req);
            break;
    }
}

void Interface::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Up:

            break;

        case Qt::Key_Down:

            break;

        case Qt::Key_Left:

            break;

        case Qt::Key_Right:

            break;
    }
}

void Interface::on_robotStart_clicked()
{
    Clientcont->connecttoRobot();
    Clientcam->connecttoRobot();
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
