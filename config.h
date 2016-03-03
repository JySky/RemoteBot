#ifndef CONFIG_H
#define CONFIG_H
#include "clientcontrol.h"
#include <QDialog>

namespace Ui {
class Config;
}
class ClientControl;

class Config : public QDialog
{
    Q_OBJECT

public:
    explicit Config(QDialog *parent = 0);
    ~Config();

private slots:
    void on_OK_clicked();

    void on_IPCamera_textChanged(const QString &arg1);

    void on_PortCamera_textChanged(const QString &arg1);

    void on_PortRobot_textChanged(const QString &arg1);

    void on_IPRobot_textChanged(const QString &arg1);

private:
    QString IPCamera;
    QString IPRobot;
    int portCamera;
    int portRobot;
    ClientControl* Clientcont;
    Ui::Config *ui;
};

#endif // CONFIG_H
