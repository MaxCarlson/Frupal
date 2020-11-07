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
    clear();
}

void Display::printMap(Camera& camera, Map& map)
{

    // Loop through each square of map. 
    // TODO: add camera checks as well as discovered/view checks later
    map.loopMap([&](int x, int y, MapSquare& sq)
    {
        char ch = sq.item ? sq.item->getCh() : ' ';

        switch(sq.terrain)
        {
            case Terrain::MEADOW:
                attron(COLOR_PAIR(Terrain::MEADOW));
                mvaddch(x, y, ch);
                attroff(COLOR_PAIR(Terrain::MEADOW));
                break;
            case Terrain::SWAMP:
                attron(COLOR_PAIR(Terrain::SWAMP));
                mvaddch(x, y, ch);
                attroff(COLOR_PAIR(Terrain::SWAMP));
                break;
            case Terrain::WATER:
                attron(COLOR_PAIR(Terrain::WATER));
                mvaddch(x, y, ch);
                attroff(COLOR_PAIR(Terrain::WATER));
                break;
            case Terrain::WALL:
                attron(COLOR_PAIR(Terrain::WALL));
                mvaddch(x, y, ch);
                attroff(COLOR_PAIR(Terrain::WALL));
                break;
            case Terrain::UNDSICOVERED:
                attron(COLOR_PAIR(Terrain::UNDSICOVERED));
                mvaddch(x, y, ch);
                attroff(COLOR_PAIR(Terrain::UNDSICOVERED));
                break;
        }
    });
}

void Display::printCharacter(Camera& camara, Player& player)
{
    // TODO: Orient camera centered on player when map position allows it 

    attron(COLOR_PAIR(Colors::PLAYER));
    mvaddch(player.getY(), player.getX(), '@');
    attroff(COLOR_PAIR(Colors::PLAYER));

    refresh();
}