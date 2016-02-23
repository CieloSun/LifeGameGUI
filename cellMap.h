#pragma once
#include "cell.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include <ctime>
#include <random>
#include <thread>

namespace cell
{
static const int MaxWidth = 2000;
static const int MaxHeight = 1000;
static const int DefaultWidth = 200;
static const int DefaultHeight = 100;

static const int FAST_SPEED=100;
static const int NORMAL_SPEED=500;
static const int SLOW_SPPED=1000;

class cellMap
{
public:


    cellMap(int = DefaultWidth, int = DefaultHeight);

    void loadMap(double = 0.5, double = 0.1, double = 0.02);
    std::vector<cell> count(int my_x, int my_y, int my_range, int ob_type, int ob_state);

    bool burn(int x, int y);

    bool eat(cell& op1, cell& op2);

    void exist(int x, int y);

    cell &cget(int x, int y)
    {
        return array[x][y];
    }

    int getWidth(){return width;}
    int getHeight(){return height;}

    static void runMap(cellMap *);

    static void pauseMap(cellMap *);

    void outputMap(std::ostream &);

    //void saveMapToFile(std::ostream &);

    //void loadMapFromFile(std::istream &);

private:
    int width;
    int height;
    double evolution;
    cell array[MaxWidth][MaxHeight];
};
}
