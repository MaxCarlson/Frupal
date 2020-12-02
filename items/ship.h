#pragma once
#include "../item.h"

class Ship : public Item
{
public:
    Ship(std::string name) 
        : Item{'S', name}
    {}

    Item::StringTuple getDescription() const
    {
        return {"Ship", "", "", ""};
    }
};