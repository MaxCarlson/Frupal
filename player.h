#pragma once
#include "tool.h"
#include <vector>



class Player
{
    int energy;
    int money;
    std::vector<Tool*> tools;
public:
    Player() :
        energy{100}, money{1000}, tools{}
    {} 
};