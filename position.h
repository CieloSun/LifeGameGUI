#pragma once
//设置位置的结构，注：=default即调用系统自动生成的构造器

namespace cell
{
struct position
{
    size_t x = 0, y = 0;
    position() = default;
    position(const position&) = default;
    position &operator=(const position&) = default;
    position(const size_t &_x, const size_t &_y) : x(_x), y(_y) {}
};
}
