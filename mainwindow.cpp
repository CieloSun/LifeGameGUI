#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "enddialog.h"
#include "savedialog.h"
#include<QFile>
#include<QString>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextDocument>

MainWindow::MainWindow(int _width, int _height, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    have_run_times=0;
    MapWidth = _width;
    MapHeight = _height*0.6;
    WIDTH = this->width()*2 / MapWidth;
    Ox = this->width() / 10;
    Oy = this->height() /6;

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
    connect(ui->actionRestart,SIGNAL(triggered(bool)),this,SLOT(Restart()));
    connect(ui->actionSetting,SIGNAL(triggered(bool)),this,SLOT(Setting()));
    connect(ui->actionPause,SIGNAL(triggered(bool)),this,SLOT(Stop()));
    connect(ui->actionEnd,SIGNAL(triggered(bool)),this,SLOT(End()));
    connect(ui->actionResume,SIGNAL(triggered(bool)),this,SLOT(Resume()));
    connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(ui->actionSave,SIGNAL(triggered(bool)),this,SLOT(Save()));
    connect(ui->actionOpen,SIGNAL(triggered(bool)),this,SLOT(Load()));

}


void MainWindow::ReStartFunction(int _sp,double p_N,double c_N,double h_N)
{
    //TODO
    //这里的设置地图方法也要按你们调试的重写以便调节图

    have_run_times=0;
//    MapWidth=_W;
//    MapHeight=_H*0.8;

//    WIDTH = this->width()*1.5 / MapWidth;
//    Ox = this->width() / 10;
//    Oy = this->height() /9;
//    int WindowWidth = this->geometry().width() * 2;
//    int WindowHeight = this->geometry().height() * 2;
//    this->resize(QSize(WindowWidth, WindowHeight));

    Mainmap = new cell::cellMap(MapWidth, MapHeight);
    Mainmap->loadMap(p_N,c_N,h_N);
    threadRun=new Thread(Mainmap);
    threadRun->setSpeed(_sp);
}

void MainWindow::Restart()
{
    threadRun->start();
    //TODO
    //利用对话框来获取数据调用RestartFunction(int,int,int,doule,double,double)
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

void MainWindow::Save()
{
    QString filename=QFileDialog::getSaveFileName(this);
    SaveFunction(filename);
    //TODO
    //建议此二SLOT设计为调用一个对话框，让用户设置文件名，进行存储，调用SaveFunction(QString fileName)

}

//void MainWindow::setCurrentFile(const QString fileName)
//{
//    //fileName=file_name;
//    text->document()->setModified(false);
//    setWindowModified(false);
//    QString titleName=fileName;
//    if(titleName.isEmpty())
//    {
//        titleName="Untitle.txt";
//    }
//    setWindowFilePath(titleName);
//}

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

void MainWindow::Load()
{
    QString name=QFileDialog::getOpenFileName(this);
    QString filename=QFileInfo(name).fileName();
    LoadFunction(filename);
}

void MainWindow::End()
{
    MyEndDialog *edialog=new MyEndDialog();
    if(threadRun->isRunning())
    {
        threadRun->stop();
    }
     edialog->show();

    //TODO
    //调出一个结果统计报告页面并询问是否存储
    //报告包括每个物种的现存数量，空地的数量，总运行次数,建议以一个对话框显示，并提供退出和重新开始两个按钮
    //如果想实时获取数据，可以把这四个变量改为类变量
    int nothing_number=threadRun->getNothingNumber();
    int producer_number=threadRun->getProducerNumber();
    int consumer_number=threadRun->getConsumerNumber();
    int high_consumer_number=threadRun->getHighConsumerNumber();

    QString nothing_text=QString::number(nothing_number);
    edialog->nothingShow->setText(nothing_text);
    QString producer_text=QString::number(producer_number);
    edialog->producerShow->setText(producer_text);
    QString consumer_text=QString::number(consumer_number);
    edialog->consumerShow->setText(consumer_text);
    QString high_consumer_text=QString::number(high_consumer_number);
    edialog->highShow->setText(high_consumer_text);

    connect(edialog->yesButton,SIGNAL(clicked()),edialog,SLOT(save()));
    connect(edialog->noButton,SIGNAL(clicked()),edialog,SLOT(close()));
}


void MainWindow::ChangeByUser(int selection)
{
    //TODO
    //通过selection的不同来对环境进行不同的设置，是否使用你们发挥一下
}

void MainWindow::mousePressEvent(QMouseEvent *)
{
    //TODO
    //利用点击时的坐标（找出一种算法）来反向计算出其表示的数组对应index，利用浮出提示/信息栏来显示出对应的信息
    //调用信息可以使用cell类中的各种get函数
}


void MainWindow::Setting()
{
    //TODO
    //同样是利用对话框（甚至是和Restart同一个对话框）来调用RestartFunction即可。
}


void MainWindow::paintEvent(QPaintEvent *)
{
    painter = new QPainter;
    painter->begin(this);
    painter->setPen(QPen(Qt::darkGreen, WIDTH / 20, Qt::DotLine));
    for (int i = 0; i <= MapHeight; ++i)
    {
        painter->drawLine(Ox, Oy + WIDTH * i, Ox + WIDTH * MapWidth, Oy + WIDTH * i);
    }
    for (int i = 0; i <= MapWidth; ++i)
    {
        painter->drawLine(Ox + WIDTH * i, Oy, Ox + WIDTH * i, Oy + WIDTH * MapHeight);
    }


    for (int i = 0; i < MapWidth; ++i)
    {
        for (int j = 0; j < MapHeight; ++j)
        {
            if(Mainmap->cget(i, j).getState() == cell::LIVE)
            {
                if (Mainmap->cget(i, j).getType() == cell::PRODUCER)
                {
                    //painter->setBrush(QBrush(Qt::green, Qt::SolidPattern));
                    //painter->drawEllipse(Ox + WIDTH * i, Oy + WIDTH * j, WIDTH / 2, WIDTH / 2);

                    QPixmap pixmap;
                    pixmap.load(":/image/glass_3.png");
                    painter->drawPixmap(Ox + WIDTH * i, Oy + WIDTH * j, WIDTH*0.7, WIDTH*0.7,pixmap);

                }
                if (Mainmap->cget(i, j).getType() == cell::CONSUMER)
                {
                    //painter->setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
                    //painter->drawEllipse(Ox + WIDTH * i, Oy + WIDTH * j, WIDTH / 2, WIDTH / 2);

                    QPixmap pixmap;
                    pixmap.load(":image/jerry.png");
                    painter->drawPixmap(Ox + WIDTH * i, Oy + WIDTH * j, WIDTH*0.8, WIDTH*0.8,pixmap);
                }
                if (Mainmap->cget(i, j).getType() == cell::HIGH_CONSUMER)
                {
                    QPixmap pixmap;
                    pixmap.load(":image/eagle_2.png");
                    painter->drawPixmap(Ox + WIDTH * i, Oy + WIDTH * j, WIDTH, WIDTH,pixmap);
                }
            }
            else if (Mainmap->cget(i, j).getState() == cell::DEAD)
            {
                if(Mainmap->cget(i, j).getType() == cell::PRODUCER)
                {
                    QPixmap pixmap;
                    pixmap.load(":image/dead_leaf_2.png");
                    painter->drawPixmap(Ox + WIDTH * i, Oy + WIDTH * j, WIDTH*0.6, WIDTH*0.6,pixmap);
                }
                else
                {
                    QPixmap pixmap;
                    pixmap.load(":image/tomb.png");
                    painter->drawPixmap(Ox + WIDTH * i, Oy + WIDTH * j, WIDTH*0.6, WIDTH*0.6,pixmap);
              }

//                painter->setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
//                painter->drawEllipse(Ox + WIDTH * i, Oy + WIDTH * j, WIDTH / 2, WIDTH / 2);

            }
        }

    }

    painter->end();
}

MainWindow::~MainWindow()
{
    delete ui;
}


//void MainWindow::on_action_connect_triggered()
//{
//    EndDialog dialog;
//    dialog.exec();
//}

