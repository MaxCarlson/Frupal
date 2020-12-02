#include "player.h"
#include "map.h"
#include <ncurses.h>

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

        // TODO: Is this what is meant by 2 squares? Probably not?
        if(hasBinoculars)
            discoverSq(x+ox*2, y+oy*2);
    }

    // TODO: Finish out binocular squares here
}

void Player::addTool(Tool *&tool)
{
    tools.push_back(tool);
}
    
bool Player::hasTools()
{
    if(tools.empty())
        return false;
    return true;
}

void Player::toggleTool()
{
    if(tools.empty())
    {
        currentTool = NULL;
        return;
    }

    if(!currentTool && !tools.empty())
    {
        currentTool = tools[0];
        return;
    }
    

    int count = 0;

    // set currentTool to next tool in tools vector
    while(tools[count])
    {
        if(currentTool == tools[count])
        {
            
            if(tools[count + 1])
            {
                currentTool = tools[count+1];
                return;
            }
            else
            {
                currentTool = tools[0];
                return;
            }
            
        }
        ++count;
    }
}
    
std::string Player::playerToolName() const
{
    std::string noTool = "None";

    if(!currentTool)
        return noTool;
    return currentTool->getName();
}

