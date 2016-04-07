#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include "ui_settingdialog.h"

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog,public Ui::SettingDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = 0);
    ~SettingDialog();

private:

};

#endif // SETTINGDIALOG_H
