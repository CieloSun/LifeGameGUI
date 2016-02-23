#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFile>
#include<QString>
#include <QMessageBox>

MainWindow::MainWindow(int _width, int _height, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    have_run_times=0;
    MapWidth = _width;
    MapHeight = _height;
    WIDTH = this->width() / MapWidth;
    Ox = this->width() / 6.5;
    Oy = this->height() / 10;

    ui->setupUi(this);
    int WindowWidth = this->geometry().width() * 2;
    int WindowHeight = this->geometry().height() * 2;
    this->resize(QSize(WindowWidth, WindowHeight));

    Mainmap = new cell::cellMap(MapWidth, MapHeight);
    Mainmap->loadMap();
    threadRun=new Thread(Mainmap);
    //TODO
    connect(threadRun,SIGNAL(ChangeScreen()),this,SLOT(Change()));
    connect(threadRun,SIGNAL(End()),this,SLOT(End()));
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
            if (Mainmap->cget(i, j).getState() == cell::LIVE)
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
    have_run_times++;
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

void MainWindow::Save()
{
    //TODO
    //建议此二SLOT设计为调用一个对话框，让用户设置文件名，进行存储，调用SaveFunction(QString fileName)
    //和LoadFunction(QString fileName)
}
void MainWindow::Load()
{
    //TODO，同上
}

void MainWindow::Resume()
{
    threadRun->resume();
    threadRun->start();
}

void MainWindow::SaveFunction(QString fileName)
{
    QFile file(fileName);
    if(!file.open((QIODevice::WriteOnly)))
    {
        std::cerr<<"Cannot open file or writing: "<<qPrintable(file.errorString())<<std::endl;
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_4);
    out<<quint32(0x12345678)<<MapWidth<<MapHeight;
    for(int i=0;i<MapWidth;++i)
    {
        for(int j=0;j<MapHeight;++j)
        {
            out<<quint32(0x12345678)<<Mainmap->cget(i,j).getType()<<'\t'<<Mainmap->cget(i,j).getState()<<'\t'
              <<Mainmap->cget(i,j).getRange()<<'\t'<<Mainmap->cget(i,j).getLiveNumber()<<'\t'
             <<Mainmap->cget(i,j).getDeadNumber()<<'\t'<<Mainmap->cget(i,j).getAgeLimit()<<'\t'
            <<Mainmap->cget(i,j).getAge()<<'\t'<<Mainmap->cget(i,j).getAfterDeadLimit()<<'\t'
            <<Mainmap->cget(i,j).getAfterDead();
        }
    }
}

void MainWindow::LoadFunction(QString fileName)
{
    QFile file(fileName);
    if(!file.open((QIODevice::ReadOnly)))
    {
        std::cerr<<"Cannot open file or reading: "<<qPrintable(file.errorString())<<std::endl;
        return;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_4);
    in>>MapWidth>>MapHeight;
    int _type,_state,_range,_liveNumber,_deadNumber,_ageLimit,_age,_afterDeadLimit,_afterDead;
    for(int i=0;i<MapWidth;++i)
    {
        for(int j=0;j<MapHeight;++j)
        {
            in>>_type>>_state>>_range>>_liveNumber>>_deadNumber>>_ageLimit>>_age>>_afterDeadLimit>>_afterDead;
            Mainmap->cget(i,j).setType(_type);
            Mainmap->cget(i,j).setState(_state);
            Mainmap->cget(i,j).setRange(_range);
            Mainmap->cget(i,j).setLiveNumber(_liveNumber);
            Mainmap->cget(i,j).setDeadNumber(_deadNumber);
            Mainmap->cget(i,j).setAgeLimit(_ageLimit);
            Mainmap->cget(i,j).setAge(_age);
            Mainmap->cget(i,j).setAfterDeadLimit(_afterDeadLimit);
            Mainmap->cget(i,j).setAfterDead(_afterDead);
        }
    }
}

void MainWindow::End()
{
    if(threadRun->isRunning())
    {
        threadRun->stop();
    }
    //TODO
    //调出一个结果统计报告页面并询问是否存储
    //报告包括每个物种的现存数量，空地的数量，总运行次数,建议以一个对话框显示，并提供退出和重新开始两个按钮
    //如果想实时获取数据，可以把这四个变量改为类变量
    int nothing_number=threadRun->getNothingNumber();
    int producer_number=threadRun->getProducerNumber();
    int consumer_number=threadRun->getConsumerNumber();
    int high_consumer_number=threadRun->getHighConsumerNumber();
    //四个数据都已经在这里获取，你们想办法利用。
}

void MainWindow::ChangeByUser(int selection)
{
    //TODO
    //通过selection的不同来对环境进行不同的设置，是否使用你们发挥一下
}



