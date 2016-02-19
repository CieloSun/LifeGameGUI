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
class cellMap
{
public:
    static const int MaxWidth = 2000;
    static const int MaxHeight = 1000;
    static const int DefaultWidth = 200;
    static const int DefaultHeight = 100;

    cellMap(int = DefaultWidth, int = DefaultHeight);

    void loadMap(double = 0.5, double = 0.1, double = 0.02);
    std::vector<cell> count(int my_x, int my_y, int my_range, int ob_type, int ob_state);
    bool burn(int x, int y);
    bool eat(cell& op1, cell& op2);
    void exist(int x, int y);
    cell cget(int x, int y)
    {
        return array[x][y];
    }
    int getWidth(){return width;}
    int getHeight(){return height;}

    static void runMap(cellMap *);

    static void pauseMap(cellMap *);
/*
    static void startMap(cellMap *);

    static void resumeMap(cellMap *);
*/
    void outputMap(std::ostream &);

    void saveMapToFile(std::ostream &);

    void loadMapFromFile(std::istream &);

private:
    int width;
    int height;
    int producerAmount;
    int consumerAmount;
    int highConsumerAmount;
    cell array[MaxWidth][MaxHeight];
    //bool pause;
};
}
