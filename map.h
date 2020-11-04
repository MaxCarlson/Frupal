#include <ncurses.h>

struct MapSquare
{
    char symbol;
    uint8_t color;

    MapSquare(char symbol, uint8_t color) 
        : symbol{symbol}, color{color}
    {}
};


class Map
{
    MapSquare map[128][128];


};