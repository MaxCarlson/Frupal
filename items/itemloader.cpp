#include "itemloader.h"
#include "../item.h"
#include "binoculars.h"
#include "chest.h"
#include "clue.h"
#include "food.h"
#include "obstacle.h"
#include "tool.h"
#include <fstream>
#include <ctime>

ItemLoader::~ItemLoader()
{
    for(auto& f : foods)
        delete f;
    for(auto& t : tools)
        delete t;
    for(auto& o : obstacles)
        delete o;
}

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
    int size = 51;

    std::ifstream file("items/items.txt");
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
                tools.emplace_back(new Tool(sname,stype,cost,rating));
            }
            else if (!sname.compare("FOOD"))
            {
                getline(file, sname, '|');
                file >> cost;
                file.ignore(size, del);
                file >> energy;
                foods.emplace_back(new Food(sname,cost,energy));
            }
            else if (!sname.compare("OBSTACLE"))
            {
                getline(file, sname, '|');
                getline(file, stype, '|');
                file >> energy;
                obstacles.emplace_back(new Obstacle(sname,stype,energy));
            }
        }

        file.ignore(size, '\n');
        getline(file, sname, '|');
    }

    file.close();
}

Binoculars ItemLoader::getBinoculars(std::default_random_engine& re) const
{
    return Binoculars{"Binoculars", 100};
}

Chest ItemLoader::getChest(std::default_random_engine& re) const
{
    static std::poisson_distribution<int> dist{4};
    return Chest{"Chest", (dist(re) + 1) * 50};
}

Clue ItemLoader::getClue(std::default_random_engine& re) const
{
    return Clue{};
}

Food ItemLoader::getFood(std::default_random_engine& re) const
{
    std::uniform_int_distribution<int> dist{0, static_cast<int>(foods.size()) - 1};
    return Food{*foods[dist(re)]};
}
Obstacle ItemLoader::getObstacle(std::default_random_engine& re) const
{
    std::uniform_int_distribution<int> dist{0, static_cast<int>(obstacles.size()) - 1};
    return Obstacle{*obstacles[dist(re)]};
}
Tool ItemLoader::getTool(std::default_random_engine& re) const
{
    std::uniform_int_distribution<int> dist{0, static_cast<int>(tools.size()) - 1};
    return Tool{*tools[dist(re)]};
}

