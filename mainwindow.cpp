#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <QMessageBox>

MainWindow::MainWindow(int _width, int _height, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    MapWidth = _width;
    MapHeight = _height;
    WIDTH = this->width() / MapWidth;
    Ox = this->width() / 6.5;
    Oy = this->height() / 10;


    Mainmap = new cell::cellMap(MapWidth, MapHeight);
    Mainmap->loadMap();
    threadRun=new Thread(Mainmap);

    ui->setupUi(this);
    int WindowWidth = this->geometry().width() * 2;
    int WindowHeight = this->geometry().height() * 2;
    this->resize(QSize(WindowWidth, WindowHeight));

    connect(threadRun,SIGNAL(ChangeScreen()),this,SLOT(Change()));
    connect(ui->actionStart, SIGNAL(triggered(bool)), this, SLOT(Start()));
    connect(ui->actionPause,SIGNAL(triggered(bool)),this,SLOT(Stop()));
    connect(ui->actionResume,SIGNAL(triggered(bool)),this,SLOT(Resume()));
    connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(ui->actionSave,SIGNAL(triggered(bool)),this,SLOT(Save()));
    connect(ui->actionOpen,SIGNAL(triggered(bool)),this,SLOT(Load()));
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    painter = new QPainter;
    painter->begin(this);
    painter->setPen(QPen(Qt::darkGreen, WIDTH / 10, Qt::DotLine));
    for (int i = 0; i <= MapHeight; ++i)
    {
        painter->drawLine(Ox, Oy + WIDTH * i, Ox + WIDTH * MapWidth, Oy + WIDTH * i);
    }
    for (int i = 0; i <= MapWidth; ++i)
    {
        painter->drawLine(Ox + WIDTH * i, Oy, Ox + WIDTH * i, Oy + WIDTH * MapHeight);
    }

    painter->setBrush(QBrush(Qt::black, Qt::SolidPattern));
    for (int i = 0; i < MapWidth; ++i)
    {
        for (int j = 0; j < MapHeight; ++j)
        {
            if (Mainmap->cget(i, j).getState() == cell::cell::LIVE)
            {
                painter->drawEllipse(Ox + WIDTH * i, Oy + WIDTH * j, WIDTH / 2, WIDTH / 2);
            }
        }
    }
    painter->end();
}

void MainWindow::Change()
{
    update();
    updateGeometry();
}

void MainWindow::Start()
{
    threadRun->start();
}

void MainWindow::Stop()
{
    if(threadRun->isRunning())
    {
        threadRun->stop();
    }
}

void MainWindow::Resume()
{
    threadRun->resume();
    threadRun->start();
}

void MainWindow::Save()
{
    //TODO

    std::fstream file("savedata.dat");
    if(file)
    {
        Mainmap->saveMapToFile(file);
        QMessageBox::information(this,"Hint",QString::fromStdString("You have save it successfully"),QMessageBox::Cancel);
    }
    file.close();

}

void MainWindow::Load()
{
    //TODO

    std::fstream file("savedata.dat");
    if(file)
    {
        Mainmap->loadMapFromFile(file);
        QMessageBox::information(this,"Hint",QString::fromStdString("You have load it successfully"),QMessageBox::Cancel);
    }
    else QMessageBox::critical(this,"Error",QString::fromStdString("No save data"),QMessageBox::Cancel);
    file.close();

}



