#include "itemloader.h"
#include "../item.h"
#include "binoculars.h"
#include "chest.h"
#include "clue.h"
#include "food.h"
#include "obstacle.h"
#include "tool.h"
#include "../mapgenerator.h"
#include "../map.h"
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

    getline(file, sname, del);

    while(file && !file.eof())
    {
        if (sname.compare("#")) // Not a comment
        {
            if (!sname.compare("TOOL"))
            {
                getline(file, sname, del);
                getline(file, stype, del);
                file >> cost;
                file.ignore(size, del);
                file >> rating;
                tools.emplace_back(new Tool(sname,stype,cost,rating));
            }
            else if (!sname.compare("FOOD"))
            {
                getline(file, sname, del);
                file >> cost;
                file.ignore(size, del);
                file >> energy;
                foods.emplace_back(new Food(sname,cost,energy));
            }
            else if (!sname.compare("OBSTACLE"))
            {
                getline(file, sname, del);
                getline(file, stype, del);
                file >> energy;
                // This comment is to fix a weird compiler problem
                obstacles.emplace_back(new Obstacle(sname,stype,energy));
            }
        }

        file.ignore(size, '\n');
        getline(file, sname, del);
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
    int val = (dist(re) + 1) * 27;
    Chest c{"Chest", val};
    return c;
}

Clue ItemLoader::getClue(std::default_random_engine& re, std::pair<int,int> clueCoords) const
{
    std::uniform_int_distribution<int> dist{0,1};   // Generate true or false
    Clue c(static_cast<bool>(dist(re)),mg,map,clueCoords);
    return c;
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

void ItemLoader::setMapInfo(MapGenerator* mapgen, Map* m)
{
    mg = mapgen;
    map = m;
}