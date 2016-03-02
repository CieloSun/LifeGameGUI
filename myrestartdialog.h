#ifndef MYRESTARTDIALOG_H
#define MYRESTARTDIALOG_H

#include <QDialog>

#include <ui_myrestartdialog.h>

class MyRestartDialog : public QDialog, public Ui::MyRestartDialog
{
    Q_OBJECT

public:
    explicit MyRestartDialog(QWidget *parent = 0);
    ~MyRestartDialog();

private:
    //Ui::MyRestartDialog *ui;
};

#endif // MYRESTARTDIALOG_H
