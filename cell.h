#pragma once
#include "position.h"

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
}
