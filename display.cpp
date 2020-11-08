#include <ncurses.h>
#include "display.h"
#include "camera.h"
#include "player.h"
#include "map.h"
#include "item.h"

enum Colors
{
    PLAYER = 1,
    UNDSICOVERED,
    MEADOW,
    SWAMP,
    WATER,
    WALL,
    TREE,
    BOULDER
};

Display::Display()
{
    // Init colors

    // Player Color {Index, Foreground, Background}
    start_color();
    init_pair(Colors::PLAYER, COLOR_YELLOW, COLOR_MAGENTA);
    init_pair(Colors::UNDSICOVERED, COLOR_BLACK, COLOR_BLACK);
    init_pair(Colors::MEADOW, COLOR_GREEN, COLOR_GREEN);
    init_pair(Colors::SWAMP, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(Colors::WATER, COLOR_BLUE, COLOR_BLUE);
    init_pair(Colors::WALL, COLOR_WHITE, COLOR_WHITE);
    //clear();
}

void Display::printMap(const Camera& camera, const Map& map)
{
    auto [dimX, dimY]       = camera.getDims();
    auto [offsetX, offsetY] = camera.getOffsets();

    // Loop through each square of map. 
    // TODO: add camera checks as well as discovered/view checks later
    map.loopMap([&](int x, int y, const MapSquare& sq)
    {
        char ch = sq.item ? sq.item->getCh() : ' ';

        // Don't print squares not covered by camera
        if(x < offsetX || x > offsetX + dimX)
            return;
        if(y < offsetY || y > offsetY + dimY)
            return;

        int printX = x - offsetX;
        int printY = y - offsetY;

        switch(sq.terrain)
        {
            case Terrain::MEADOW:
                attron(COLOR_PAIR(Terrain::MEADOW));
                mvaddch(printY, printX, ch);
                attroff(COLOR_PAIR(Terrain::MEADOW));
                break;
            case Terrain::SWAMP:
                attron(COLOR_PAIR(Terrain::SWAMP));
                mvaddch(printY, printX, ch);
                attroff(COLOR_PAIR(Terrain::SWAMP));
                break;
            case Terrain::WATER:
                attron(COLOR_PAIR(Terrain::WATER));
                mvaddch(printY, printX, ch);
                attroff(COLOR_PAIR(Terrain::WATER));
                break;
            case Terrain::WALL:
                attron(COLOR_PAIR(Terrain::WALL));
                mvaddch(printY, printX, ch);
                attroff(COLOR_PAIR(Terrain::WALL));
                break;
            case Terrain::UNDSICOVERED:
                attron(COLOR_PAIR(Terrain::UNDSICOVERED));
                mvaddch(printY, printX, ch);
                attroff(COLOR_PAIR(Terrain::UNDSICOVERED));
                break;
        }
    });
}

void Display::printCharacter(const Camera& camera, const Player& player)
{
    // TODO: Orient camera centered on player when map position allows it 
    //auto [dimX, dimY]       = camera.getDims();
    auto [offsetX, offsetY] = camera.getOffsets();

    int printX = player.getX() - offsetX;
    int printY = player.getY() - offsetY;

    attron(COLOR_PAIR(Colors::PLAYER));
    mvaddch(printY, printX, '@');
    attroff(COLOR_PAIR(Colors::PLAYER));

    refresh();
}