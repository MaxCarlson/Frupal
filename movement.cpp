#include "movement.h"
#include "player.h"
#include "map.h"
#include "input.h"
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
    Input input;

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

    if(sq.item)
    {
        // item is food
        if(dynamic_cast<Food*>(sq.item))
        {
            Food *food = dynamic_cast<Food*>(sq.item);

            // player chooses to buy food and can afford to do so
            if(input.buyItem() && player.getMoney() >= food->getCost())
            {
                player.modifyMoney(-food->getCost());
                player.modifyEnergy(food->getEnergy());

                food = nullptr;
                delete sq.item;
                sq.item = nullptr;
            }
            else
              return;

            // if player doesn't have enough money, maybe inform the player?
        }

        if(dynamic_cast<Obstacle*>(sq.item))
        {
            Obstacle *obstacle = dynamic_cast<Obstacle*>(sq.item);
            if(input.canBreakObstacle(player, obstacle->getEnergy()))
            {
                obstacle = nullptr;
                delete sq.item;
                sq.item = nullptr;
            }

            else
            {
                player.modifyEnergy(-obstacle->getEnergy());
                return;
            }
                
            // Check if player can afford to remove obstacle
            // either with current energy or energy combined with tool
            //
            // If the player can't afford it...kill player
            // If the player can affor it, remove obstacle

        }

        if(dynamic_cast<Tool*>(sq.item))
        {
            Tool *tool = dynamic_cast<Tool*>(sq.item);
            if(input.buyItem() && player.getMoney() >= tool->getCost())
            {
                // put tool in player's tool belt
                player.modifyMoney(-tool->getCost());
                player.addTool(tool);
                
                tool = nullptr;
                delete sq.item;
                sq.item = nullptr;
            }
            return;
        }

        if(dynamic_cast<Binoculars*>(sq.item))
        {
            Binoculars *binoculars = dynamic_cast<Binoculars*>(sq.item);
            if(input.buyItem() && player.getMoney() >= binoculars->getCost())
            {
                player.modifyMoney(-binoculars->getCost());
                player.boughtBinoculars();
                binoculars = nullptr;
                delete sq.item;
                sq.item = nullptr;

            }
            return;
        }
    }
    
    player.setX(xf);
    player.setY(yf);
            
}
