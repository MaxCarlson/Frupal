#pragma once
#include "camera.h"
#include <random> // Just for testing

static std::default_random_engine re;
static std::uniform_int_distribution<int> distr(1, 4);

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

    //MapSquare() = default;
    MapSquare() : terrain{static_cast<Terrain>(distr(re))}, item{} {} // Just for testing!

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
    std::pair<int, int> getXY() const { return {width, height}; }
    const MapSquare& sq(int x, int y) const { return map[y][x]; }

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