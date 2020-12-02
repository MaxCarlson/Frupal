#pragma once
#include "../item.h"

class Ship : public Item
{
    int cost;

public:
    Ship(std::string name) 
        : Item{'S', name, 50}               // Default cost
    {}

    Ship(std::string name, int cost) 
        : Item{'S', name}, cost{cost}       // Option for custom cost
    {}

    Item::StringTuple getDescription() const
    {
        std::string l1;
        static const std::string cost - "> Cost: ";
        l1 = cost + std::to_string(getCost());
        
        return StringTuple{l1, "", "", ""};
    }

    int getCost() const { return cost; }
};