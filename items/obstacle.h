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
    std::string getType() const { return type; }

    int match(std::string toolType)
    {
        return !type.compare(toolType);
    }

    StringTuple getDescription() const
    {
        std::string l1, l2;
        static const std::string type   = "> Type: ";
        static const std::string energy = "> Energy Cost: ";
        l1 = type   + getType();
        l2 = energy + std::to_string(getEnergy());

        return StringTuple{l1, l2, "", ""}; 
    }
};

