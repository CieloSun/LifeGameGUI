#include "settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    QPixmap pixmap= QPixmap(":image/bg7.png").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background,QBrush(pixmap));
    this->setPalette(palette);
}

SettingDialog::~SettingDialog()
{

}
