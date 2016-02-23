#include "thread.h"
#include<QMutexLocker>



Thread::Thread(cell::cellMap* _Mainmap):Mainmap(_Mainmap)
{
    stopped=false;
    speed=cell::NORMAL_SPEED;
}

void Thread::setMap(cell::cellMap* _Mainmap)
{
    Mainmap=_Mainmap;
}

void Thread::stop()
{
    QMutexLocker locker(&mutex);
    stopped=true;
}

void Thread::setSpeed(int _speed)
{
    QMutexLocker locker(&mutex);
    speed=_speed;
}

void Thread::resume()
{
    QMutexLocker locker(&mutex);
    stopped=false;
}

void Thread::run()
{
    forever
    {
        QMutexLocker locker(&mutex);
        if(stopped)
        {
            stopped=false;
            break;
        }

        for (int i = 0; i < Mainmap->getWidth(); ++i)
        {
            for (int j = 0; j < Mainmap->getHeight(); ++j)
            {
                if (Mainmap->cget(i,j).getType() == cell::NOTHING)
                {
                    Mainmap->burn(i, j);
                }
                else
                {
                    Mainmap->exist(i, j);
                }
            }
        }
        emit ChangeScreen();
        Sleep(speed);

    }
}
