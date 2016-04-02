#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{

    ui->setupUi(this);
    QPixmap mypix (":/image/image/about.png");
    ui->label->setPixmap(mypix);
    this->setWindowTitle("About");
    this->setAttribute(Qt::WA_DeleteOnClose );
}

About::~About()
{
    delete ui;
}
