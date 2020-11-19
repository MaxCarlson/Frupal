#include "movement.h"
#include "player.h"
#include "map.h"


void Movement::movePlayer(Player& player, Map& map, int x, int y)
{
    int xf = player.getX() + x;
    int yf = player.getY() + y;

    const MapSquare& sq = map.sq(xf, yf);

    switch(sq.terrain)
    {
        case Terrain::UNDISCOVERED:
            break; // Should never happen
        case Terrain::MEADOW:
            player.modifyEnergy(-1);
            break;
        case Terrain::SWAMP:
            player.modifyEnergy(-2);
            break;

        case Terrain::WATER: // TODO: Handle ship and remove drop-through
        case Terrain::WALL:
            player.modifyEnergy(-1);
            return;
    }

    player.setX(xf);
    player.setY(yf);
}
