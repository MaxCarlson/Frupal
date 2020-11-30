#pragma once
#include "../item.h"

class Clue : public Item
{
public:
    Clue() = default;
    Clue(std::string name) 
        : Item{'?', name}
    {}
};