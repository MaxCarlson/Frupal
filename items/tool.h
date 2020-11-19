#pragma once
#include "../item.h"

class Tool : public Item
{
public:
    Tool(std::string name) 
        : Item{'T', name}
    {}
};
