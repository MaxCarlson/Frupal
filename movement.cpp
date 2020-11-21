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

    MapSquare& sq = map.sq(xf, yf);

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
        // item is food
        if(dynamic_cast<Food*>(sq.item))
        {
            Food *food = dynamic_cast<Food*>(sq.item);

            // player chooses to buy food and can afford to do so
            if(player.buyFood() && player.getMoney() >= food->getCost())
            {
                player.modifyMoney(-food->getCost());
                player.modifyEnergy(food->getEnergy());

                food = nullptr;
                delete sq.item;
                sq.item = nullptr;
            }

            // if player doesn't have enough money, maybe inform the player?
        }
    }
    


        
}
