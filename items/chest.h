#pragma once
#include "../item.h"


class Chest : public Item
{
    int value;
public:
    Chest() = default;
    Chest(std::string name, int value) 
        : Item{'$', name}, value{value}
    {}

    Item::StringTuple getDescription() const 
    {
        return {"Treasure", "$" + std::to_string(value), "", ""};
    }
};