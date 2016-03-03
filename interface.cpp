#include "interface.h"
#include "ui_interface.h"
#include <QKeyEvent>

Interface::Interface(QWidget *parent) : QMainWindow(parent), ui(new Ui::Interface)
{
    ui->setupUi(this);

    Clientcont = ClientControl::getInstance(this);
    connect(ui->menuConfigurer, SIGNAL(aboutToShow()), this, SLOT(displayConfig()));

}

void Interface::displayConfig()
{
    Config* window= new Config();
    window->exec();

}
void Interface::connectionState(bool test)
{
    if(test)
        this->setStyleSheet("QLabel#colorConnected { background-color : green;}");
    else
        this->setStyleSheet("QLabel#colorConnected  { background-color : red;}");
}

void Interface::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Up:

            break;

        case Qt::Key_Down:

            break;

        case (Qt::Key_Left):

            break;

        case (Qt::Key_Right):

            break;

        case Qt::Key_Z:
            req.setUrl((QUrl(QString("http://"+Clientcont->getIp()+":"+Clientcont->getPort()+"/?action=command&dest=0&plugin=0&id=10094853&group=1&value=-200"))));
            mgr.get(req);
            break;

        case Qt::Key_S:
            req.setUrl((QUrl(QString("http://"+Clientcont->getIp()+":"+Clientcont->getPort()+"/?action=command&dest=0&plugin=0&id=10094853&group=1&value=200"))));
            mgr.get(req);
            break;

        case Qt::Key_Q:
            req.setUrl((QUrl(QString("http://"+Clientcont->getIp()+":"+Clientcont->getPort()+"/?action=command&dest=0&plugin=0&id=10094852&group=1&value=200"))));
            mgr.get(req);
            break;

        case Qt::Key_D:
            req.setUrl((QUrl(QString("http://"+Clientcont->getIp()+":"+Clientcont->getPort()+"/?action=command&dest=0&plugin=0&id=10094852&group=1&value=-200"))));
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
}


Interface::~Interface()
{
    delete ui;
}




