#pragma once
#include "../item.h"

class Obstacle : public Item
{
    std::string type;   // Tool's type should match
    int energy;         // Energy cost to remove

public:

    Obstacle(std::string name, std::string type, int energy) :
        Item{'!', name}, type{type}, energy{energy}
    {}

    int getEnergy() const { return energy; }
    
    int match(std::string toolType)
    {
        return !type.compare(toolType);
    }
};

