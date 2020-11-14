#include "map.h"

Map::Map(int width, int height) :
    width{width}, height{height}
{
    map = new MapSquare*[height];

    // Default map squares to undiscovered
    for(int i = 0; i < height; ++i)
        map[i] = new MapSquare[width]();
}

Map::~Map()
{
    if(!map)
        return;
    for(int i = 0; i < height; ++i)
        delete[] map[i];
    delete[] map;
}
