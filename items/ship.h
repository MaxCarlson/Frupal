#pragma once
#include "../item.h"

class Ship : public Item
{
    //int cost; 

public:
    
    Ship(std::string name):
         Item{'S', name}
    {}
    
    //int getCost()   const { return cost; }

    StringTuple getDescription() const
    {
        std::string l1;   
        static const std::string ship   = "> Ship: ";
        //static const std::string cost   = "> Cost: ";
        
        l1 = ship + getName(); 
        //l2 = cost + std::to_string(getCost()); 

        return StringTuple{l1, "", "", ""}; 
    }
};
