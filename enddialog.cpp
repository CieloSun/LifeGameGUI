#include "enddialog.h"
#include <QString>

MyEndDialog::MyEndDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);


    QPixmap pixmap= QPixmap(":image/bg5.png").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background,QBrush(pixmap));
    this->setPalette(palette);

//    setWindowFlags(Qt::FramelessWindowHint);
//    setAttribute(Qt::WA_DeleteOnClose);
//    ui->saveLabel->setStyleSheet("color:#ffffff");
//    ui->yesButton->setStyleSheet("background-color:#5e7950;color:#ffffff;border:0px");

}

MyEndDialog::~MyEndDialog()
{
}
