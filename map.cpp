#include "map.h"
#include "item.h"

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
    {
        for(int j = 0; j < width; ++j)
            delete map[i][j].item;
        delete[] map[i];
    }
    delete[] map;
}

void Map::resetMap(int widthToLoad, int heightToLoad) {
    if(!map)
        return;
    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
            delete map[i][j].item;
        delete[] map[i];
    }
    delete[] map;

    height = heightToLoad;
    width = widthToLoad; 

    map = new MapSquare*[height];

    // Default map squares to undiscovered
    for(int i = 0; i < height; ++i)
        map[i] = new MapSquare[width]();

  return;
}
