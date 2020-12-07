#include <ncurses.h>
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


void Movement::movePlayer(Player& player, Map& map, UI& ui, Camera& camera, int x, int y)
{
    int xf = player.getX() + x;
    int yf = player.getY() + y; 
    int menuOffset = COLS - 21; // a bit hacky but a quick way to be able to update UI
    Input input;

    MapSquare& sq       = map.sq(xf, yf);
    MapSquare& startSq  = map.sq(player.getX(), player.getY());

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

        case Terrain::WATER:
            // Moving onto a ship costs energy
            if(dynamic_cast<Ship*>(sq.item) && startSq.terrain != Terrain::WATER)
                player.modifyEnergy(-1);
            else if(dynamic_cast<Ship*>(startSq.item))
            {}
            else
            {
                player.modifyEnergy(-1);
                return;
            }

            // If the player is already on a ship, move the ship with the player if we''re still on water
            if(dynamic_cast<Ship*>(startSq.item) && !sq.item)
            {
                sq.item = startSq.item;
                startSq.item = nullptr;
            }
 
            break;
        case Terrain::WALL:
            player.modifyEnergy(-1);
            return;
    }

    if(sq.item)
    {
    
        curs_set(1);

        auto [l1, l2, l3, l4] = sq.item->getDescription();


        mvaddstr(1, menuOffset, l1.c_str());
        mvaddstr(2, menuOffset, l2.c_str());
        mvaddstr(3, menuOffset, l3.c_str());
        mvaddstr(4, menuOffset, l4.c_str());
    
        curs_set(0);

        // item is food
        if(dynamic_cast<Food*>(sq.item))
        {
            Food *food = dynamic_cast<Food*>(sq.item);
            // player chooses to buy food and can afford to do so
            if(input.buyItem(camera, ui) && player.getMoney() >= food->getCost())
            {
                player.modifyMoney(-food->getCost());
                player.modifyEnergy(food->getEnergy());

                delete sq.item;
                sq.item = nullptr;
            }
            else
              return; // if player doesn't have enough money, maybe inform the player?
        }

        if(dynamic_cast<Obstacle*>(sq.item))
        {
            Obstacle *obstacle = dynamic_cast<Obstacle*>(sq.item);
            if(input.canBreakObstacle(player, obstacle, obstacle->getEnergy()))
            {
                delete sq.item;
                sq.item = nullptr;
            }
            else
                return;
        }

        if(dynamic_cast<Tool*>(sq.item))
        {
            Tool *tool = dynamic_cast<Tool*>(sq.item);
            if(input.buyItem(camera, ui) && player.getMoney() >= tool->getCost())
            {
                // put tool in player's tool belt
                player.modifyMoney(-tool->getCost());
                player.addTool(tool);
                
                delete sq.item;
                sq.item = nullptr;
            }
            else
              return;
        }

        if(dynamic_cast<Binoculars*>(sq.item))
        {
            Binoculars *binoculars = dynamic_cast<Binoculars*>(sq.item);
            if(input.buyItem(camera, ui) && player.getMoney() >= binoculars->getCost())
            {
                player.modifyMoney(-binoculars->getCost());
                player.boughtBinoculars();
                binoculars = nullptr;
                delete sq.item;
                sq.item = nullptr;
            }
            else
              return;
        }

        if(dynamic_cast<Chest*>(sq.item))
        {
            Chest *chest = dynamic_cast<Chest*>(sq.item);
            player.modifyMoney(chest->getValue());
            delete sq.item;
            sq.item = nullptr;
        }
        
        if(dynamic_cast<Diamond*>(sq.item))
        {
            player.modifyMoney(1000000);
            delete sq.item;
            sq.item = nullptr;
        }
    }
    player.setX(xf);
    player.setY(yf);
}


