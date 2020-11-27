#pragma once
#include "../item.h"


class Chest : public Item
{
public:
    Chest() = default;
    Chest(std::string name) 
        : Item{'$', name}
    {}
};