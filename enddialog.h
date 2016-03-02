#ifndef MYENDDIALOG_H
#define MYENDDIALOG_H

#include <QDialog>
#include <QString>

#include <ui_enddialog.h>
class MyEndDialog : public QDialog, public Ui::EndDialog
{
    Q_OBJECT
public:
    explicit MyEndDialog(QWidget *parent = 0);
    ~MyEndDialog();

private:

};

#endif // ENDDIALOG_H
