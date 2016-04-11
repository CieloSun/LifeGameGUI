#include "aboutusdialog.h"
#include "ui_aboutusdialog.h"

aboutUsDialog::aboutUsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutUsDialog)
{
    ui->setupUi(this);
    setWindowOpacity(0.8);
}

aboutUsDialog::~aboutUsDialog()
{
    delete ui;
}
