#pragma once
#include"cell.h"
#include<random>
#include<atomic>
#include<thread>
#include<iostream>

namespace cell {
using namespace std;
//地图类
class cellMap
{
public:
    //默认构造器初始化时，finish和flush的状态
    static constexpr int FLUSH_INIT = -1;
    static constexpr int FLUSH_FINISH = 0;

    cellMap()
    {
        finish = false;
        flush = -1;
        //补充arr中每个cell的初始化
        for (unsigned int i = 0;i < getSize();++i)
        {
            for (unsigned int j = 0;j < getSize();++j)
            {
                arr[i][j] = cell(position(i, j), DEAD);
            }
        }
        //若pos属性删除，此段代码不必要
    }
    cellMap(const cellMap&) = default;
    cellMap &operator=(const cellMap&) = default;
    cellMap(const size_t _size) : size(_size)
    {
        cellMap();
    }

    void setSize(const size_t _size)
    {
        size = _size;
    }

    const size_t& getSize() const
    {
        return size;
    }

    //Summary: Get reference of cell
    cell& get(const position &curr)
    {
        return arr[curr.x][curr.y];
    }
    //Summary: Get constant reference of cell
    const cell& cget(const position &curr) const
    {
        return arr[curr.x][curr.y];
    }

    //Summary: Get the number of living neighbours.
    int count(const position &curr) const
    {
        int res = -cget(curr).state;
        for (size_t i = -1; i <= 1; ++i)
        {
            for (size_t j = -1; j <= 1; ++j)
            {
                res += cget(position(curr.x + i, curr.y + j)).state;
            }
        }
        return res;
    }
    int count(const cell &curr) const
    {
        return count(curr.pos);
    }

    static void nextCell(cellMap *currMap, const position curr)
    {
        int num = currMap->count(curr);
        cell *targetCell = &(currMap->get(curr));
        cell nextCell;
        while (!currMap->finish)
        {
            nextCell = *targetCell;
            if (num < BurnNum || LiveNum < LiveNum)
            {
                nextCell.state = DEAD;
            }
            else if (num == LiveNum)
            {
                nextCell.state = LIVE;
            }
            while (currMap->flush == FLUSH_INIT || currMap->flush == FLUSH_FINISH)
            {
                this_thread::yield();
            }
            *targetCell = nextCell;
            --currMap->flush;
            while (currMap->flush != FLUSH_INIT)
            {
                this_thread::yield();
            }

        }
        --currMap->flush;
    }

    void nextMap()
    {
        flush = getSize() * getSize();
        while (flush != FLUSH_FINISH)
        {
            this_thread::yield();
        }
        flush = FLUSH_INIT;
    }

    void startMap()
    {
        finish = false;
        flush = FLUSH_INIT;
        position pos;
        for (pos.x = 0; pos.x < getSize(); ++pos.x)
        {
            for (pos.y = 0; pos.y < getSize(); ++pos.y)
            {
                thread(nextCell, this, pos).detach();
            }
        }
    }

    void stopMap()
    {
        finish = true;
        flush = getSize() * getSize();
        while (flush != FLUSH_FINISH)
        {
            this_thread::yield();
        }
        flush = FLUSH_INIT;
        finish = false;
    }

    //Load Map from filestream or screen(stdin)
    void loadMap(istream &ins)
    {
        //暂时先写从文件读入
        while (!ins.eof())
        {
            ins >> size;
            position pos;
            int pos_state;
            for (pos.x = 0; pos.x < getSize(); ++pos.x)
            {
                for (pos.y = 0; pos.y < getSize(); ++pos.y)
                {
                    ins >> pos_state;
                    arr[pos.x][pos.y] = cell(pos, pos_state);
                }
            }
            //bool pfinish;
            //bool pflush;
            //ins >> pfinish >> pflush >> stat;
            //finish = pfinish;
            //flush = pflush;
        }
    }

    //Save Map to filestream or screen(stdout)
    void saveMap(ostream &file)
    {
        //输出到文件
        if (file.good())
        {
            file << getSize() << endl;
            position pos;
            for (pos.x = 0; pos.x < getSize(); ++pos.x)
            {
                for (pos.y = 0; pos.y < getSize(); ++pos.y)
                {
                    file << arr[pos.x][pos.y].state << " ";
                }
                file << endl;
            }
            //bool pfinish = finish;
            //bool pflush = flush;
            //file << pfinish << " " << pflush << " " << stat << endl;
        }
    }

    /*Summary: Generate Map by generator
                Parameters:
                        seed: Seed for random engine
                        freq: Possibility of live cells between 0 and 1
                Return: True if loaded successfully
                */
    bool loadMap(double freq, const unsigned int seed)
    {
        if (freq < 0 || freq > 1) return false;
        std::default_random_engine engine(seed);
        std::uniform_real_distribution<double> distribution(0, 1);
        position pos;
        for (pos.x = 0; pos.x < getSize(); ++pos.x)
        {
            for (pos.y = 0; pos.y < getSize(); ++pos.y)
            {
                if (distribution(engine) < freq)
                {
                    get(pos).state = LIVE;
                }
                else
                {
                    get(pos).state = DEAD;
                }
            }
        }
        return true;
    }

    //Summary: Fill Map with a cell object
    void loadMap(const cell obj)
    {
        position pos;
        for (pos.x = 0; pos.x < getSize(); ++pos.x)
        {
            for (pos.y = 0; pos.y < getSize(); ++pos.y)
            {
                get(pos) = obj;
            }
        }
    }

private:
    size_t size;
    cell arr[MaxSize][MaxSize];

    //True if the progress will be finished
    atomic<bool> finish;

    /*
                FLUSH_INIT	Not flush
                FLUSH_FINISH	Complete flush
                >0		Number of unflushed cells
    */
    atomic<int> flush;
    int stat = 0;
};

}
