#pragma once
#include "camera.h"

class Item;

// Must use like: Terrain::Meadow, this is to keep them getting mixed up with the color enums in display
enum class Terrain
{
    UNDSICOVERED,
    MEADOW,
    SWAMP,
    WATER,
    WALL
};

struct MapSquare
{
    Terrain terrain;
    Item* item;

    MapSquare() = default;
    MapSquare(Terrain terrain, Item* item) 
        : terrain{terrain}, item{item}
    {}
};

class Map
{
    int width, height;
    MapSquare** map;

public:
    Map(int width, int height);
    ~Map();

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    // Func is a lambda
    template<class Func>
    void loopMap(const Func& func) const
    {
        // Width and height are inverted in order here because array access is faster like this
        for(int i = 0; i < height; ++i)
            for(int j = 0; j < width; ++j)
                func(j, i, map[i][j]);
    }

};