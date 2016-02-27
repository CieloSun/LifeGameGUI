#ifndef MYSAVEDIALOG_H
#define MYSAVEDIALOG_H

#include <QDialog>
#include <QString>
#include <ui_savedialog.h>

class MySaveDialog : public QDialog,public Ui::SaveDialog
{
    Q_OBJECT

public:
    explicit MySaveDialog(QWidget *parent = 0);
    ~MySaveDialog();

};

#endif // SAVEDIALOG_H
