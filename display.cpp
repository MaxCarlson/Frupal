#include <ncurses.h>
#include <assert.h>
#include "display.h"
#include "camera.h"
#include "player.h"
#include "map.h"
#include "item.h"
#include "ui.h"

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

void Display::printMap(const Camera& camera, const Map& map, const UI& ui)
{
    auto [dimX, dimY]       = camera.getDims();
    auto [offsetX, offsetY] = camera.getOffsets();

    // Discover adjacent terrain
    clear();

    // Loop through each square of map. 
    // TODO: add camera checks as well as discovered/view checks later
    map.loopMap([&](int x, int y, const MapSquare& sq)
    {
        char ch = sq.item ? sq.item->getCh() : ' ';

        // Don't print squares not covered by camera
        if(x < offsetX || x > offsetX + dimX - ui.getSize())
            return;
        if(y < offsetY || y > offsetY + dimY)
            return;

        int printX = x - offsetX;
        int printY = y - offsetY;

        if(sq.discovered)
            switch(sq.terrain)
            {
                case Terrain::MEADOW:
                    attron(COLOR_PAIR(Colors::MEADOW));
                    mvaddch(printY, printX, ch);
                    attroff(COLOR_PAIR(Colors::MEADOW));
                    break;
                case Terrain::SWAMP:
                    attron(COLOR_PAIR(Colors::SWAMP));
                    mvaddch(printY, printX, ch);
                    attroff(COLOR_PAIR(Colors::SWAMP));
                    break;
                case Terrain::WATER:
                    attron(COLOR_PAIR(Colors::WATER));
                    mvaddch(printY, printX, ch);
                    attroff(COLOR_PAIR(Colors::WATER));
                    break;
                case Terrain::WALL:
                    attron(COLOR_PAIR(Colors::WALL));
                    mvaddch(printY, printX, ch);
                    attroff(COLOR_PAIR(Colors::WALL));
                    break;
                case Terrain::UNDISCOVERED: // Get rid of error
                default:
                    break;
            }
        else
        {
            attron(COLOR_PAIR(Terrain::UNDISCOVERED));
            mvaddch(printY, printX, ch);
            attroff(COLOR_PAIR(Terrain::UNDISCOVERED));
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
}

void Display::printUI(const Camera& camera, UI& ui, const Player& player, Map& map)
{
    ui.print(*this, player, camera, map);
    refresh();
}

void Display::printCeneteredText(int x1, int x2, int y, std::string text)
{
    assert(static_cast<int>(text.length()) <= x2 - x1);
    mvaddstr(y, (x2 - x1) / 2 - text.length() / 2, text.c_str());
}
