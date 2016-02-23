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

    ~Thread()=default;

signals:
    void ChangeScreen();
protected:
    void run();


private:
    volatile int speed;
    volatile bool stopped;
    cell::cellMap* Mainmap;
    QMutex mutex;
};

#endif // THREAD_H
