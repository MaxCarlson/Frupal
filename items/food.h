#pragma once
#include "../item.h"


class Food : public Item
{
    int cost;
    int energy;

public:
    Food(std::string name, int cost, int energy) :
        Item{'F', name}, cost{cost}, energy{energy}
    {}

    int getCost()   const { return cost; }
    int getEnergy() const { return energy; }
    
    StringTuple getDescription() const
    {
        std::string l1, l2, l3; 
        static const std::string food   = "> Food: ";
        static const std::string cost   = "> Cost: ";
        static const std::string energy = "> Energy: ";
        l1 = food   + getName();
        l2 = cost   + std::to_string(getCost());
        l3 = energy + std::to_string(getEnergy());

        return std::tuple{l1, l2, l3}; 
    }
};
