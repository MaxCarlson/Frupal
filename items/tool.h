#pragma once
#include "../item.h"

class Tool : public Item
{

    std::string type;   // Obstacle's type should match
    int cost;           // Amt in whiffles to purchase
    int rating;         // Effectiveness (1x, 2x, 3x, etc.)

public:

    // Full constructor
    Tool(std::string name, std::string type, int cost, int rating)
        : Item{'T', name}, type{type}, cost{cost}, rating{rating}
    {}

    int match(std::string obsType)
    {
        return !type.compare(obsType);
    }

    std::string getType() const { return type; }
    int getCost()   const { return cost; }
    int getRating() const { return rating; }
    
    Item::StringTuple getDescription() const
    {
        return Item::StringTuple{"Name: "+ name, "Type: " + type, "Cost: " + std::to_string(cost), "Rating: " + std::to_string(rating)};

    }



};

