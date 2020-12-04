#include "player.h"
#include "map.h"
#include <ncurses.h>
#include "items/tool.h"
#include "items/obstacle.h"

std::pair<int, int> Player::selectedSquare() const
{
    switch(dir)
    {
        case NORTH:
            return {x, y-1};
        case EAST:
            return {x+1, y};
        case SOUTH:
            return {x, y+1};
        case WEST:
            return {x-1, y};
    }
    return {-1, -1};
}

void Player::discoverTerrrain(Map& map)
{
    constexpr std::pair<int, int> dirs[] = 
        {{0,0}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}};

    auto [xMax, yMax] = map.getXY();

    for(auto [ox, oy] : dirs)
    {
        auto discoverSq = [&](int xf, int yf)
        {
            if(xf < 0 || yf < 0 || xf > xMax - 1 || yf > yMax - 1)
                return;

            MapSquare& sq = map.sq(xf, yf);
            sq.discovered = true;
        };

        discoverSq(ox+x, oy+y);

        if(hasBinoculars) 
        {
            discoverSq(x+ox*2, y+oy*2);
            discoverSq(x+ox, y+oy*2);
            discoverSq(x+ox*2, y+oy);
        }    
    }
}

void Player::addTool(Tool *tool)
{
    tools.push_back(tool);
}
    
bool Player::hasTools()
{
    if(tools.empty())
        return false;
    return true;
}

int Player::toggleTool()
{
    if(tools.empty())
        return toolIDX;

    ++toolIDX;
    if(toolIDX >= static_cast<int>(tools.size()))
        toolIDX = 0;

    return toolIDX; 
}
    
std::string Player::playerToolName() const
{
    std::string noTool = "None";

    if(tools.empty())
        return noTool;
    return tools[toolIDX]->getName() + " (x" + std::to_string(tools[toolIDX]->getRating()) + ")";
}

int Player::useTool(Obstacle *obstacle)
{
    if(tools.empty())
        return -1;
    int toolRating = 0;

    if(obstacle->getType() == tools[toolIDX]->getType())
    {
        toolRating = tools[toolIDX]->getRating();
        tools.erase(tools.begin() + toolIDX);
        toolIDX = 0;
        return toolRating;
    }

    return -1;
}
    
bool Player::toolTypeMatch(Obstacle * obstacle)
{
    if(tools.empty())
        return false;
    if(obstacle->getType() == tools[toolIDX]->getType())
        return true;
    return false;  
}


