#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTimer>
#include "enddialog.h"
#include "cellMap.h"
#include "ui_mainwindow.h"
#include "thread.h"
#include "savedialog.h"
#include "myrestartdialog.h"
#include <QString>
#include <QPlainTextEdit>

//class EndDialog;

namespace cell
{
class MainWindow;
}

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int = cell::DefaultWidth , int = cell::DefaultHeight , QWidget *parent = 0); //TODO
    ~MainWindow();
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);//TODO
private slots:
    void Change();
    void Start();
    void Restart();//TODO
    void Stop();
    void Resume();
    void End();//TODO
    void ChangeByUser(int selection);//TODO
    void Save();//TODO
    void Load();//TODO
    void Setting();//TODO
    //void setCurrentFile(const QString fileName);
    //void okSlot();

private:
    void SaveFunction(QString fileName);
    void LoadFunction(QString fileName);
    void ReStartFunction(int = cell::NORMAL_SPEED, double = 0.5, double = 0.1, double = 0.02); //TODO
    int MapWidth;
    int MapHeight;
    QPlainTextEdit *text;

    Ui::MainWindow *ui;
    QPainter *painter;
    cell::cellMap *Mainmap;
    Thread *threadRun;
    MyEndDialog *edialog;
    MyRestartDialog *sdialog;


    int WIDTH;
    int Ox;
    int Oy;
    unsigned long long have_run_times;
};

#endif // MAINWINDOW_H
