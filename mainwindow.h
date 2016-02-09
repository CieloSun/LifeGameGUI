#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include "cell.h"
#include "ui_mainwindow.h"

namespace cell {
class MainWindow;
}

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    const int Ox=20;
    const int Oy=50;

    explicit MainWindow(int _SIZE,QWidget *parent = 0);
    ~MainWindow();
protected:
    void paintEvent(QPaintEvent *);
private slots:
    void drawTheArr();
private:
    Ui::MainWindow *ui;
    QPainter *painter;
    cell::cellMap *Mainmap;
    int SIZE;
    int WIDTH;
};

#endif // MAINWINDOW_H
