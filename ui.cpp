#include "ui.h"
#include "display.h"
#include "player.h"
#include "camera.h"
#include "map.h"
#include <ncurses.h>

void UI::print(Display& display, const Player& player, const Camera& camera, Map& map)
{
    printOutline(display, camera);
    auto [cx, cy] = camera.getDims();


    // Offset for all printed text in UI
    int xOffset = cx - cols + 2;


    //std::string cost = ""



    mvaddstr(4, xOffset, "Options:");
    mvaddstr(5, xOffset, "1) North");
    mvaddstr(6, xOffset, "2) East");
    mvaddstr(7, xOffset, "3) South");
    mvaddstr(8, xOffset, "4) West");

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

    //move(0, COLS-cols);
    move(0, xpos);
    vline('#', 128);
}

void UI::printSelectedInfo(const Player& player, Map& map, const Camera& camera, int xOffset)
{
    auto [cx, cy] = camera.getDims();
    auto [sx, sy] = player.selectedSquare();

    if(sx < 0 || sy < 0 || sx > cx - 1 || sy > cy - 1)
    {
        curs_set(0);
        return;
    }
    else
        curs_set(1);

    // Set the cursor to its given pos
    move(sy, sx);

    const MapSquare& sq =  map.sq(sx, sy);

    if(!sq.item)
        return;

    std::string food = "> Food: ";
    
}