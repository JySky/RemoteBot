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

    signals:
        void getIPCam();
        void getPortCam();
        void getIPRobot();
        void getPortRobot();
        void setIPCam(QString);
        void setPortCam(int);
        void setIPRobot(QString);
        void setPortRobot(int);

    private slots:
        void on_IPCamera_textChanged(const QString &arg1);
        void on_PortCamera_textChanged(const QString &arg1);
        void on_PortRobot_textChanged(const QString &arg1);
        void on_IPRobot_textChanged(const QString &arg1);
        void on_buttonBox_accepted();

    public slots:
        void receiveIPCam(QString ipc);
        void receivePortCam(int portc);
        void receiveIPRobot(QString ipr);
        void receivePortRobot(int portr);

    private:
        QString IPCamera;
        QString IPRobot;
        QString URLCamera;
        int portCamera;
        int portRobot;
        ClientControl* Clientcont;
        ClientCamera* Clientcam;
        Ui::Config *ui;
        void majConfig();
};

#endif // CONFIG_H
