#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTimer>
#include "cellMap.h"
#include "ui_mainwindow.h"
#include "thread.h"
#include <QString>

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
    explicit MainWindow(int , int , QWidget *parent = 0);//TODO
    ~MainWindow();
protected:
    void paintEvent(QPaintEvent *);
private slots:
    void Change();
    void Start();
    void Stop();
    void Resume();
    void End();//TODO
    void ChangeByUser(int selection);//TODO
    void Save();//TODO
    void Load();//TODO
private:
    void SaveFunction(QString fileName);
    void LoadFunction(QString fileName);
    int MapWidth;
    int MapHeight;

    Ui::MainWindow *ui;
    QPainter *painter;
    cell::cellMap *Mainmap;
    Thread *threadRun;
    int WIDTH;
    int Ox;
    int Oy;
    unsigned long long have_run_times;
};

#endif // MAINWINDOW_H
