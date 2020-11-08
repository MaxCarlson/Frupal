#include "ui.h"
#include "display.h"
#include "player.h"
#include "camera.h"
#include <ncurses.h>

void UI::print(Display& display, const Player& player, const Camera& camera)
{
    printOutline(display, camera);
    auto [cx, cy] = camera.getDims();

    // Offset for all printed text in UI
    int xOffset = cx - cols + 2;

    std::string food = "> Food: ";
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