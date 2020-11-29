#pragma once
#include "../item.h"


class Chest : public Item
{
    int value;
public:
    Chest() = default;
    Chest(std::string name, int value) 
        : Item{'$', name}
    {}
};