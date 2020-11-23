#pragma once
#include "../item.h"

class Binoculars : public Item
{
    int cost;
public:
    Binoculars(std::string name, int cost) 
        : Item{'B', name}, cost{cost}
    {}

    int getCost() const { return cost; }
};
