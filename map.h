#pragma once
#include "camera.h"



struct MapSquare
{
    char symbol;
    uint8_t color;

    MapSquare() = default;
    MapSquare(char symbol, uint8_t color) 
        : symbol{symbol}, color{color}
    {}
};

class Map
{

    MapSquare map[maxMapHeight][maxMapWidth];

public:


};