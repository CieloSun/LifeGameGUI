#include "myrestartdialog.h"
#include "ui_myrestartdialog.h"

MyRestartDialog::MyRestartDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    QPixmap pixmap= QPixmap(":image/bg7.png").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background,QBrush(pixmap));
    this->setPalette(palette);
}

MyRestartDialog::~MyRestartDialog()
{
    //delete ui;
}
