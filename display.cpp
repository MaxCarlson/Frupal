#include <ncurses.h>
#include <assert.h>
#include "display.h"
#include "camera.h"
#include "player.h"
#include "map.h"
#include "item.h"
#include "ui.h"
#include "items/diamond.h"
#include <string.h>

enum Colors
{
    PLAYER = 1,
    UNDSICOVERED,
    MEADOW,
    SWAMP,
    WATER,
    WALL,
    DIAMOND
};

Display::Display()
{
    start_color();
    init_pair(Colors::PLAYER, COLOR_YELLOW, COLOR_MAGENTA);
    init_pair(Colors::UNDSICOVERED, COLOR_BLACK, COLOR_BLACK);
    init_pair(Colors::MEADOW, COLOR_BLACK, COLOR_GREEN);
    init_pair(Colors::SWAMP, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(Colors::WATER, COLOR_WHITE, COLOR_BLUE);
    init_pair(Colors::WALL, COLOR_WHITE, COLOR_WHITE);
    init_pair(Colors::DIAMOND, COLOR_WHITE, COLOR_CYAN);
}

void Display::printMap(const Camera& camera, const Map& map, const UI& ui)
{
    auto [dimX, dimY]       = camera.getDims();
    auto [offsetX, offsetY] = camera.getOffsets();

    erase();

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

        if(!sq.discovered)
            return;

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

        if(dynamic_cast<Diamond*>(sq.item))
        {
            attron(COLOR_PAIR(Colors::DIAMOND));
            mvaddch(printY, printX, ch);
            attroff(COLOR_PAIR(Colors::DIAMOND));
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

void Display::printCenteredText(int x1, int x2, int y, std::string text)
{
    assert(static_cast<int>(text.length()) <= x2 - x1);
    mvaddstr(y, (x2 - x1) / 2 - text.length() / 2, text.c_str());
}

int Display::deathScreen()
{
    int ch = 0;
    char s1[] = "You have died!";
    char s2[] = "Press 'S' to start a new game or 'Q' to quit.";
    int x = (LINES / 2);
    int y = (COLS - strlen(s1)) / 2;

    erase();
    // Prints messages in center of screen
    mvprintw(x, y, "%s", s1);
    y = (COLS - strlen(s2)) / 2;
    ++y;
    ++x;
    mvprintw(x, y, "%s", s2);

    timeout(250); // Wait 250ms for keypress
    while(ch != 's' && ch != 'q')
        ch = getch();

    if(ch == 'q')
        return 0;

    return 1;
}

int Display::winScreen()
{
    int ch = 0;
    char s1[] = "You have found the Royal Diamond!";
    char s2[] = "You Win!";
    char s3[] = "Press 'S' to start a new game or 'Q' to quit.";
    int x = (LINES / 2);
    int y = (COLS - strlen(s1)) / 2;


    erase();
    // Prints messages in center of screen
    mvprintw(x, y, "%s", s1);
    y = (COLS - strlen(s2)) / 2;
    ++y;
    ++x;
    mvprintw(x, y, "%s", s2);
    y = (COLS - strlen(s3)) / 2;
    ++y;
    ++x;
    mvprintw(x, y, "%s", s3);

    timeout(250); // Wait 250ms for keypress
    while(ch != 's' && ch != 'q')
        ch = getch();

    if(ch == 'q')
        return 0;

    return 1;
}



