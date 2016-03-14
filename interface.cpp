#include "interface.h"
#include "ui_interface.h"
#include <QKeyEvent>

Interface::Interface(QWidget *parent) : QMainWindow(parent), ui(new Ui::Interface)
{
    ui->setupUi(this);
    Clientcont = ClientControl::getInstance(this);
    Clientcam = ClientCamera::getInstance(this);
    //ui->webView->load(QUrl("http://192.168.1.106:8080/javascript_simple.html"));
}


void Interface::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_I:
                Clientcont->setLeftSpeedFlag(1);
                Clientcont->setRightSpeedFlag(1);
                Clientcont->setLeftSpeed((int)(ui->displaySpeedSet->value()*2.4));
                Clientcont->setRightSpeed((int)(ui->displaySpeedSet->value()*2.4));
            break;

        case Qt::Key_K:
                Clientcont->setLeftSpeedFlag(1);
                Clientcont->setRightSpeedFlag(1);
                Clientcont->setLeftSpeed((int)(ui->displaySpeedSet->value()*2.4));
                Clientcont->setRightSpeed((int)(ui->displaySpeedSet->value()*2.4));
            break;

        case Qt::Key_J:
                Clientcont->setLeftSpeedFlag(1);
                Clientcont->setRightSpeedFlag(1);

            break;

        case Qt::Key_L:
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
        case Qt::Key_I:

            break;

        case Qt::Key_K:

            break;

        case Qt::Key_J:

            break;

        case Qt::Key_L:

            break;
    }
}

void Interface::on_robotStart_clicked()
{
    Clientcont->connecttoRobot();
    Clientcam->connecttoCamera();
    //ui->webView->load(QUrl("http://192.168.1.106:8080/javascript_simple.html"));
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

void Interface::on_robotStop_clicked()
{
    Clientcont->stopConnectionRobot();
    this->setcolorConnected("red");
}
void Interface::majInterface(RobotInfo dataR, RobotInfo dataL)
{
    ui->batteryLevel->setValue((int)(dataL.getBatLevel()/2.55));
    ui->displaySpeed->display(5);

}
