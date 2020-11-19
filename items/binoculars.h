#pragma once
#include "../item.h"

class Binoculars : public Item
{
public:
    Binoculars(std::string name) 
        : Item{'B', name}
    {}
};
