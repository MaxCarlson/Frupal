#pragma once
#include "../item.h"

class Tool : public Item
{

    std::string type;   // Obstacle's type should match
    int cost;           // Amt in whiffles to purchase
    int rating;         // Effectiveness (1x, 2x, 3x, etc.)

public:
    Tool(std::string name, int cost, int rating) 
        : Item{'T', name}, cost{cost}, rating{rating}
    {}

    // Full constructor
    Tool(std::string name, std::string type, int cost, int rating)
        : Item{'T', name}, type{type}, cost{cost}, rating{rating}
    {}

    // Rating = 1 by default
    Tool(std::string name, std::string type, int cost)
        : Item{'T', name}, type{type}, cost{cost}, rating{1}
    {}

    int match(std::string obsType)
    {
        return !type.compare(obsType);
    }
    int getCost()   const { return cost; }
    int getRating() const { return rating; }

};

