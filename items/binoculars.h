#pragma once
#include "../item.h"

class Binoculars : public Item
{
    int cost;

public:
    Binoculars(std::string name, int cost) 
        : Item{'B', name}, cost{cost}
    {}
    
    int getCost() const { return cost; }
    
    StringTuple getDescription() const
    {
        std::string l1, l2, l3;  
        static const std::string Binoculars  = ">Binoculars: ";
        static const std::string cost   = ">Cost: ";
        l1 = Binoculars   + getName(); 
        l2 = cost   + std::to_string(getCost());

        return std::tuple{l1, l2, "", ""}; 
    }
};
