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
    // Non binocular dirs
    constexpr std::pair<int, int> dirs[] = 
        {{0,0}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}};

    // TODO: Binocular check
    //if(hasBinoculars)

    auto [dx, dy] = map.getXY();

    for(auto [ox, oy] : dirs)
    {
        ox += x;
        oy += y;

        if(ox < 0 || oy < 0 || ox > dx || oy > dy)
            continue;

        MapSquare& sq = map.sq(ox, oy);
        sq.discovered = true;
    }
}