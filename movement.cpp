#include "movement.h"
#include "player.h"
#include "map.h"
#include "items/binoculars.h"
#include "items/chest.h"
#include "items/clue.h"
#include "items/diamond.h"
#include "items/food.h"
#include "items/obstacle.h"
#include "items/ship.h"
#include "items/tool.h"
#include "items/itemloader.h"


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

    if(sq.item)
    {
        // Item is food
        //if(dynamic_cast<Food*>(sq.item))
        // Allow player to buy or not buy food
        // If no, do nothing
        // If yes, subtract corresponding money from player,
        // Add corresponding energy to player
        // delete food
           
    }


        
}
