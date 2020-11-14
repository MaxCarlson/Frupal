#pragma once
#include "../item.h"


class Chest : public Item
{
public:
    Chest(std::string name) 
        : Item{'$', name}
    {}
};