#pragma once
#include "../item.h"

class Ship : public Item
{
public:
    Ship(std::string name) 
        : Item{'S', name}
    {}
};