#pragma once
#include "../item.h"

class Obstacle : public Item
{
public:
    Obstacle(std::string name) 
        : Item{'!', name}
    {}
};