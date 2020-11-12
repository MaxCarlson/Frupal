#pragma once
#include "../item.h"

//enum obType {ROCK, TREE} type; // Put this in Item? For use in tools as well

class Obstacle : public Item {};

/*
{
    int energy; // Energy cost to remove

public:
    Obstacle(std::string name, int energy, obType type) :
        Item{'!', name}, energy{energy}, type{type}
    {}

    obType getType() const { return type; }
    int getEnergy() const { return energy; }
};
*/