#include "config.h"
#include "ui_config.h"
#include <QDebug>

Config::Config(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::Config)
{
    ui->setupUi(this);

    Clientcont = ClientControl::getInstance();
    Clientcam = ClientCamera::getInstance();
    QObject::connect(this, SIGNAL(getIPCam()),Clientcam, SLOT(getIP()), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(getPortCam()),Clientcam, SLOT(getPort()), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(getIPRobot()),Clientcont, SLOT(getIP()), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(getPortRobot()),Clientcont, SLOT(getPort()), Qt::QueuedConnection);
    QObject::connect(Clientcam, SIGNAL(sendIP(QString)),this, SLOT(receiveIPCam(QString)), Qt::QueuedConnection);
    QObject::connect(Clientcam, SIGNAL(sendPort(int)),this, SLOT(receivePortCam(int)), Qt::QueuedConnection);
    QObject::connect(Clientcont, SIGNAL(sendIP(QString)),this, SLOT(receiveIPRobot(QString)), Qt::QueuedConnection);
    QObject::connect(Clientcont, SIGNAL(sendPort(int)),this, SLOT(receivePortRobot(int)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(setIPCam(QString)),Clientcam, SLOT(setIP(QString)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(setPortCam(int)),Clientcam, SLOT(setPort(int)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(setIPRobot(QString)),Clientcont, SLOT(setIP(QString)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(setPortRobot(int)),Clientcont, SLOT(setPort(int)), Qt::QueuedConnection);

    emit getIPCam();
    emit getPortCam();
    emit getIPRobot();
    emit getPortRobot();
    this->setAttribute(Qt::WA_DeleteOnClose );
}

Config::~Config()
{
    delete ui;
}

void Config::majConfig()
{
    ui->IPCamera->setText(IPCamera);
    ui->IPRobot->setText(IPRobot);
    ui->PortRobot->setText(QString::number(portRobot));
    ui->PortCamera->setText(QString::number(portCamera));
}

void Config::on_IPCamera_textChanged(const QString &arg1)
{
    IPCamera=arg1;
}

void Config::on_PortCamera_textChanged(const QString &arg1)
{
    QString temp=arg1;
    portCamera=temp.split(" ")[0].toInt();
}

void Config::on_PortRobot_textChanged(const QString &arg1)
{
    QString temp=arg1;
    portRobot=temp.split(" ")[0].toInt();
}

void Config::on_IPRobot_textChanged(const QString &arg1)
{
    IPRobot=arg1;
}

void Config::receiveIPCam(QString ipc)
{
    IPCamera=ipc;
    majConfig();
}

void Config::receivePortCam(int portc)
{
    portCamera=portc;
    majConfig();
}

void Config::receiveIPRobot(QString ipr)
{
    IPRobot=ipr;
    majConfig();
}

void Config::receivePortRobot(int portr)
{
    portRobot=portr;
    majConfig();
}

void Config::on_buttonBox_accepted()
{
    emit setIPCam(IPCamera);
    emit setPortCam(portCamera);
    emit setIPRobot(IPRobot);
    emit setPortRobot(portRobot);
}
