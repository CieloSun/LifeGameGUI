#include "myrestartdialog.h"
#include "ui_myrestartdialog.h"

MyRestartDialog::MyRestartDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

MyRestartDialog::~MyRestartDialog()
{
    //delete ui;
}
