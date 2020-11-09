#include "player.h"
#include "map.h"

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

    auto [dx, dy] = map.getXY();

    for(auto [ox, oy] : dirs)
    {
        ox += x;
        oy += y;

        auto discoverSq = [&](int ox, int oy)
        {
            if(ox < 0 || oy < 0 || ox > dx || oy > dy)
                return;

            MapSquare& sq = map.sq(ox, oy);
            sq.discovered = true;
        };
        
        discoverSq(ox, oy);

        if(hasBinoculars)
            discoverSq(ox*2, oy*2);
    }
}