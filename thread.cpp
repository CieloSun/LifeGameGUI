#include "thread.h"
#include<QMutexLocker>

Thread::Thread(cell::cellMap* _Mainmap):Mainmap(_Mainmap)
{
    stopped=false;
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
                if (Mainmap->cget(i,j).getType() == cell::cell::NOTHING)
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
        Sleep(500);

    }
}
