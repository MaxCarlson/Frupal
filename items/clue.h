#pragma once
#include "../item.h"

class Clue : public Item
{
public:
    Clue(std::string name) 
        : Item{'?', name}
    {}
};