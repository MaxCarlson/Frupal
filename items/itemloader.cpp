#include "itemloader.h"
#include "../item.h"
#include "chest.h"
#include "clue.h"
#include "food.h"
#include "obstacle.h"
#include "tool.h"
#include <fstream>
#include <ctime>

void ItemLoader::loadItems()
{
    /* File should be named "items.txt"
     * Syntax:
     * 
     * #|Comment
     * TOOL|Tool Name|TYPE|cost|rating
     * FOOD|Food Name|cost|energy
     * OBSTACLE|Obstacle Name|TYPE|energy
     * 
     * std::vector<Food>       foods;
     * std::vector<Tool>       tools;
     * std::vector<Obstacle>   obstacles;
     */

    char del = '|';         // Delimiter
    std::string sname;
    std::string stype;
    int cost = 0;
    int rating = 0;
    int energy = 0;
    Item * item_ptr = NULL; // All-purpose pointer
    int size = 51;

    std::ifstream file("items.txt");
    if (!file) return;

    getline(file, sname, '|');

    while(file && !file.eof())
    {
        if (sname.compare("#")) // Not a comment
        {
            if (!sname.compare("TOOL"))
            {
                getline(file, sname, '|');
                getline(file, stype, '|');
                file >> cost;
                file.ignore(size, del);
                file >> rating;
                item_ptr = new Tool(sname,stype,cost,rating);
                tools.push_back(*(dynamic_cast<Tool*>(item_ptr)));
            }
            else if (!sname.compare("FOOD"))
            {
                getline(file, sname, '|');
                file >> cost;
                file.ignore(size, del);
                file >> energy;
                item_ptr = new Food(sname,cost,energy);
                foods.push_back(*(dynamic_cast<Food*>(item_ptr)));
            }
            else if (!sname.compare("OBSTACLE"))
            {
                getline(file, sname, '|');
                getline(file, stype, '|');
                file >> energy;
                item_ptr = new Obstacle(sname,stype,energy);
                obstacles.push_back(*(dynamic_cast<Obstacle*>(item_ptr)));
            }
            
            // Reset the pointer for the next item
            item_ptr = NULL;
        }

        file.ignore(size, '\n');
        getline(file, sname, '|');
    }

    file.close();
}

Chest* ItemLoader::getChest() const
{
    return nullptr;
}

Clue* ItemLoader::getClue() const
{
    return nullptr;
}

Food* ItemLoader::getFood() const
{
    std::srand(std::time(nullptr));
    int rand = std::rand() % foods.size();
    Food * const foodptr = const_cast<Food*>(&foods[rand]);

    return foodptr;
}
Obstacle* ItemLoader::getObstacle() const
{
    std::srand(std::time(nullptr));
    int rand = std::rand() % obstacles.size();
    Obstacle * const obstacleptr = const_cast<Obstacle*>(&obstacles[rand]);

    return obstacleptr;
}
Tool* ItemLoader::getTool() const
{

    std::srand(std::time(nullptr));
    int rand = std::rand() % tools.size();
    Tool * const toolptr = const_cast<Tool*>(&tools[rand]);

    return toolptr;
}

