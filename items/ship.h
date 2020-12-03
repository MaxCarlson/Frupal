#pragma once
#include "../item.h"

class Ship : public Item
{
    int cost;

public:
    Ship(std::string name) 
        : Item{'S', name}, cost{50}         // Default cost
    {}

    Ship(std::string name, int cost) 
        : Item{'S', name}, cost{cost}       // Option for custom cost
    {}

    Item::StringTuple getDescription() const
    {
        std::string l1, l2;
        static const std::string cstr = "> Cost: ";
        l1 = "> " + getName();
        l2 = cstr + std::to_string(getCost());
        
        return StringTuple{l1, l2, "", ""};
    }

    int getCost() const { return cost; }
};