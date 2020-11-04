#pragma once
#include "camera.h"

enum Obstacle
{
    NONE = 0,
    TREE,
    BOULDER,
};


struct MapSquare
{
    char symbol;
    uint8_t color;
    Obstacle obstacle;

    MapSquare() = default;
    MapSquare(char symbol, uint8_t color, Obstacle obstacle) 
        : symbol{symbol}, color{color}, obstacle{obstacle}
    {}
};

class Map
{

    MapSquare map[maxMapHeight][maxMapWidth];

public:


};