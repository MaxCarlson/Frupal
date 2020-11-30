#include "ui.h"
#include "display.h"
#include "player.h"
#include "camera.h"
#include "map.h"
#include "item.h"
#include "items/binoculars.h"
#include "items/chest.h"
#include "items/clue.h"
#include "items/diamond.h"
#include "items/food.h"
#include "items/obstacle.h"
#include "items/ship.h"
#include "items/tool.h"
#include <ncurses.h>

void UI::print(Display& display, const Player& player, const Camera& camera, Map& map)
{
    printOutline(display, camera);
    auto [cx, cy] = camera.getDims();


    // Offset for all printed text in UI
    int xOffset = cx - cols + 2;

    mvaddstr(5, xOffset, "Options:");
    mvaddstr(6, xOffset, "1) North");
    mvaddstr(7, xOffset, "2) East");
    mvaddstr(8, xOffset, "3) South");
    mvaddstr(9, xOffset, "4) West");

    std::string wifs = "Whiffles: " + std::to_string(player.getMoney());
    std::string ener = "Energy:   " + std::to_string(player.getEnergy());

    mvaddstr(cy-3, xOffset, wifs.c_str());
    mvaddstr(cy-2, xOffset, ener.c_str());

    printSelectedInfo(player, map, camera, xOffset);
}

void UI::printOutline(Display& display, const Camera& camera)
{
    auto [cx, cy] = camera.getDims();

    // TODO: Eventually make this so if the terminal is larger than the map, 
    // the UI rests at the edge of the map instead of the edge of the screen?
    int xpos = cx - cols;

    move(0, xpos);
    vline('#', 128);
}

void UI::printSelectedInfo(const Player& player, Map& map, const Camera& camera, int xOffset)
{
    auto [cx, cy]   = camera.getDims();
    auto [cxo, cyo] = camera.getOffsets();
    auto [sx, sy]   = player.selectedSquare();

    if(sx < 0 || sy < 0 || sx > cx - 1 || sy > cy - 1)
    {
        curs_set(0);
        return;
    }
    else
        curs_set(1);

    // TODO: This is non-functional right now for the right half of the map
    // Set the cursor to its given pos
    move(sy - cyo, sx - cxo);

    const MapSquare& sq =  map.sq(sx, sy);

    if(!sq.item)
        return;

    // TODO: Add all other item types in here
    auto [l1, l2, l3, l4] = sq.item->getDescription();

    mvaddstr(1, xOffset, l1.c_str());
    mvaddstr(2, xOffset, l2.c_str());
    mvaddstr(3, xOffset, l3.c_str());
    mvaddstr(4, xOffset, l4.c_str());

    curs_set(0);
}

