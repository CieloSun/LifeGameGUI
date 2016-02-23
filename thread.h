#ifndef THREAD_H
#define THREAD_H
#include "cellMap.h"
#include "windows.h"

#include<QMutex>
#include<QThread>
class Thread :public QThread
{
    Q_OBJECT
public:
    explicit Thread(cell::cellMap* _Mainmap);
    void setMap(cell::cellMap* _Mainmap);

    void stop();
    void resume();
    void setSpeed(int _speed);
    int getNothingNumber() const;
    int getProducerNumber() const;
    int getConsumerNumber() const;
    int getHighConsumerNumber() const;


    ~Thread()=default;

signals:
    void ChangeScreen();
    //因为突变的加入，不存在死循环的可能，End信号用来发射细胞数为空的情况。
    void End();
protected:
    void run();


private:
    volatile int nothing_number;
    volatile int producer_number;
    volatile int consumer_number;
    volatile int high_consumer_number;
    volatile int speed;
    volatile bool stopped;
    cell::cellMap* Mainmap;
    QMutex mutex;
};

#endif // THREAD_H
