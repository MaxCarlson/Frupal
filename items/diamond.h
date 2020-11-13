#pragma once
#include "../item.h"

class Diamond : public Item
{
public:
    Diamond(std::string name) 
        : Item{'$', name}
    {}
};