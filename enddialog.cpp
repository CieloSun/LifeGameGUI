#include "enddialog.h"
#include <QString>

MyEndDialog::MyEndDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    //connect(yesButton,SIGNAL(clicked))
}

MyEndDialog::~MyEndDialog()
{
}
