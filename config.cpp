#include "config.h"
#include "ui_config.h"

Config::Config(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::Config)
{
    ui->setupUi(this);
    Clientcont = ClientControl::getInstance(NULL);
}

Config::~Config()
{
    delete ui;
}



void Config::on_OK_clicked()
{
    Clientcont->setIp(IPRobot);
    Clientcont->setPort(portRobot);
    connect(ui->OK, SIGNAL(clicked()), this, SIGNAL(closeThisWindow()));
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
