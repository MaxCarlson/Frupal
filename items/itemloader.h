#pragma once
#include <vector>
#include <random>

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
public:

    ~ItemLoader();

    // Load all items from text file
    void loadItems();

    // Generate a random item of each type
    Chest getChest(std::default_random_engine& re) const;
    Clue getClue(std::default_random_engine& re) const;
    Food getFood(std::default_random_engine& re) const;
    Obstacle getObstacle(std::default_random_engine& re) const;
    Tool getTool(std::default_random_engine& re) const;
};