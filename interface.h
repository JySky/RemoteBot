#ifndef INTERFACE_H
#define INTERFACE_H
#include "clientcontrol.h"
#include "config.h"
#include <QMainWindow>

namespace Ui {
class Interface;
}
class ClientControl;
class Interface : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Interface(QWidget *parent = 0);
        ~Interface();

    private:
        QNetworkAccessManager mgr;
        QNetworkRequest req;
        Ui::Interface *ui;
        ClientControl *Clientcont;
    protected:
        void keyPressEvent(QKeyEvent *event);
        void keyReleaseEvent(QKeyEvent *event);
    private slots:
        void displayConfig();
        void connectionState(bool test);
        void on_robotStart_clicked();

};

#endif // INTERFACE_H
