#ifndef CONFIG_H
#define CONFIG_H
#include "clientcontrol.h"
#include "clientcamera.h"
#include <QDialog>

namespace Ui {
class Config;
}
class ClientControl;
class ClientCamera;

class Config : public QDialog
{
    Q_OBJECT

    public:
        explicit Config(QDialog *parent = 0);
        ~Config();

    private slots:

        void on_IPCamera_textChanged(const QString &arg1);

        void on_PortCamera_textChanged(const QString &arg1);

        void on_PortRobot_textChanged(const QString &arg1);

        void on_IPRobot_textChanged(const QString &arg1);

        void on_buttonBox_accepted();

private:
        QString IPCamera;
        QString IPRobot;
        int portCamera;
        int portRobot;
        ClientControl* Clientcont;
        ClientCamera* Clientcam;
        Ui::Config *ui;
};

#endif // CONFIG_H
