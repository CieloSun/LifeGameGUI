#pragma once
#include<random>
#include<atomic>
#include<thread>
#include<iostream>

using namespace std;

namespace cell
{

//周围八单位中，至少存在2个活单位时，可出生，当超过3个时，会死亡
//注：constexpr即static const
constexpr int BurnNum = 2;
constexpr int LiveNum = 3;
//调整到此位置因为QT编译器自身的一些问题
constexpr int DEAD = 0;
constexpr int LIVE = 1;
//最大限制为1000
constexpr size_t MaxSize = 1000;

struct position
{
    size_t x = 0, y = 0;
    position() = default;
    position(const position&) = default;
    position &operator=(const position&) = default;
    position(const size_t &_x, const size_t &_y) : x(_x), y(_y) {}
};

//设置状态，种类变量，提供（横轴，纵轴，状态，种类）的构造器
struct cell
{
    position pos;
    int state = 0, type = 0;
    cell() = default;
    cell(const cell&) = default;
    cell &operator=(const cell&) = default;
    cell(const position &_pos, const int &_state = 0, const int &_type = 0) : pos(_pos), state(_state), type(_type) {}
    cell(const size_t &_x, const size_t &_y, const int &_state = 0, const int &_type = 0) : cell(position(_x, _y), _state, _type) {}
};

//地图类
class cellMap
{
public:
    //默认构造器初始化时，finish和flush的状态
    static constexpr int FLUSH_INIT = -1;
    static constexpr int FLUSH_FINISH = 0;

    cellMap();

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
    int count(const position &curr) const;

    int count(const cell &curr) const
    {
        return count(curr.pos);
    }

    static void nextCell(cellMap *currMap, const position curr);

    void nextMap();

    void startMap();

    void stopMap();

    void loadMap(istream &ins);

    void saveMap(ostream &file);

    bool loadMap(double freq, const unsigned int seed);

    void loadMap(const cell obj);



private:
    size_t size;
    cell arr[2*MaxSize][MaxSize];

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
