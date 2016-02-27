#include "savedialog.h"
#include "ui_savedialog.h"
#include <QString>

MySaveDialog::MySaveDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

MySaveDialog::~MySaveDialog()
{
    //delete ui;
}
