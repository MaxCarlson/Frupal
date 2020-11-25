#include "player.h"
#include "map.h"
#include <ncurses.h>

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
