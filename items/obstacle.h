#pragma once
#include "../item.h"

class Obstacle : public Item
{
    int energy; // Energy cost to remove

public:
    Obstacle(std::string name, int energy) :
        Item{'!', name}, energy{energy}
    {}

    int getEnergy() const { return energy; }
};

