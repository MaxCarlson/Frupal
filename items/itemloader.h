#pragma once
#include <vector>

class Chest;
class Clue;
class Food;
class Obstacle;
class Tool;

class ItemLoader
{
    std::vector<Food>       foods;
    std::vector<Tool>       tools;
    std::vector<Obstacle>   obstacles;
public:

    // Load all items from text file
    void loadItems();

    // Generate a random item of each type
    Chest* getChest() const;
    Clue* getClue() const;
    Food* getFood() const;
    Obstacle* getObstacle() const;
    Tool* getTool() const;
};