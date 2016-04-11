#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTimer>
#include "enddialog.h"
#include "cellMap.h"
#include "ui_mainwindow.h"
#include "thread.h"
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
    void mousePressEvent(QMouseEvent *);
private slots:
    void Change();
    void Start();
    void Restart();
    void Stop();
    void Resume();
    void End();
    void Save();
    void Load();
    void Setting();
    void Test();
    void damageFunction();
    void fineFunction();
    void aboutUs();

private:
    void SaveFunction(QString fileName);
    void LoadFunction(QString fileName);
    void ReStartFunction(double = 0.5, double = 0.1, double = 0.02); //TODO

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
    int HEIGHT;
    int Ox;
    int Oy;
    unsigned long long have_run_times;
    std::default_random_engine engine;

    bool damage;
};

#endif // MAINWINDOW_H
