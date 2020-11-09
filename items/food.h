#pragma once
#include "../item.h"


class Food : public Item
{
    int cost;
    int energy;
public:
    Food(std::string name, int cost, int energy) :
        Item{'F', name}, cost{cost}, energy{energy}
    {}

    int getCost()   const { return cost; }
    int getEnergy() const { return energy; }
};