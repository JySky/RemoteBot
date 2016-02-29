#ifndef INTERFACE_H
#define INTERFACE_H
#include "clientcontrol.h"
#include "config.h"
#include <QMainWindow>

namespace Ui {
class Interface;
}

class Interface : public QMainWindow
{
    Q_OBJECT

public:
    explicit Interface(QWidget *parent = 0);
    ~Interface();

private:
    Ui::Interface *ui;
};

#endif // INTERFACE_H
