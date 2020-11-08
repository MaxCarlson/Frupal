#pragma once
#include "items/tool.h"
#include <vector>



class Player
{
    int energy;
    int money;
    int x, y;
    std::vector<Tool*> tools;
public:
    Player() :
        energy{100}, money{1000}, x{0}, y{0}, tools{}
    {} 

    int getX() const { return x; }
    int getY() const { return y; }
    void setX(int v) { x = v; }
    void setY(int v) { y = v; }
    std::pair<int, int> getXY() const { return {x, y}; }

};