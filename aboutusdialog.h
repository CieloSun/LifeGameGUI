#ifndef ABOUTUSDIALOG_H
#define ABOUTUSDIALOG_H

#include <QDialog>

namespace Ui {
class aboutUsDialog;
}

class aboutUsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit aboutUsDialog(QWidget *parent = 0);
    ~aboutUsDialog();

private:
    Ui::aboutUsDialog *ui;
};

#endif // ABOUTUSDIALOG_H
