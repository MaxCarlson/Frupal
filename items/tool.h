#pragma once
#include "../item.h"

class Tool : public Item
{
    int cost;
    int rating;
public:
    Tool(std::string name, int cost, int rating) 
        : Item{'T', name}, cost{cost}, rating{rating}
    {}

    int getCost()   const { return cost; }
    int getRating() const { return rating; }
};
