#pragma once
#include "items/tool.h"
#include <vector>

enum Direction
{
    NORTH,
    EAST,
    SOUTH,
    WEST
};


class Player
{
    int energy;
    int money;
    int x, y;
    Direction dir;
    std::vector<Tool*> tools;
public:
    Player() :
        energy{100}, money{1000}, x{0}, y{0}, dir{SOUTH}, tools{}
    {} 

    int getX() const { return x; }
    int getY() const { return y; }
    void setX(int v) { x = v; }
    void setY(int v) { y = v; }

    int getMoney()  const { return money; }
    int getEnergy() const { return energy; }
    Direction getDir() const { return dir; }
    void setDir(Direction dr) { dir = dr; }
    std::pair<int, int> selectedSquare() const;

    std::pair<int, int> getXY() const { return {x, y}; }

};