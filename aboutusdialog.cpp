#include "aboutusdialog.h"
#include "ui_aboutusdialog.h"

aboutUsDialog::aboutUsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutUsDialog)
{
    ui->setupUi(this);
    //setWindowOpacity(0.8);
    QPixmap pixmap= QPixmap(":image/bg6.png").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background,QBrush(pixmap));
    this->setPalette(palette);
}

aboutUsDialog::~aboutUsDialog()
{
    delete ui;
}
