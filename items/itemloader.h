#pragma once
#include <vector>
#include <random>
#include "../mapgenerator.h"

class Binoculars;
class Chest;
class Clue;
class Food;
class Obstacle;
class Tool;

class ItemLoader
{
    std::vector<Food*>       foods;
    std::vector<Tool*>       tools;
    std::vector<Obstacle*>   obstacles;
    MapGenerator* mg;

public:

    ~ItemLoader();

    // Load all items from text file
    void loadItems();

    // Get a pointer to mapgenerator
    void setMapGen(MapGenerator*);

    // Generate a random item of each type
    Chest getChest(std::default_random_engine& re) const;
    Clue getClue(std::default_random_engine& re, std::pair<int,int> clueCoords) const;
    Food getFood(std::default_random_engine& re) const;
    Obstacle getObstacle(std::default_random_engine& re) const;
    Tool getTool(std::default_random_engine& re) const;
    Binoculars getBinoculars(std::default_random_engine& re) const;

    template<class ItemType>
    ItemType getItem(std::default_random_engine& re, std::pair<int,int> clueCoords) const
    {
        if constexpr(std::is_same_v<ItemType, Binoculars>)
            return getBinoculars(re);
        if constexpr(std::is_same_v<ItemType, Chest>)
            return getChest(re);
        if constexpr(std::is_same_v<ItemType, Clue>)
            return getClue(re, clueCoords);
        if constexpr(std::is_same_v<ItemType, Food>)
            return getFood(re);
        if constexpr(std::is_same_v<ItemType, Obstacle>)
            return getObstacle(re);
        if constexpr(std::is_same_v<ItemType, Tool>)
            return getTool(re);
    }
};