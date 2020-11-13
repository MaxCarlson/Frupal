#pragma once
#include "camera.h"
#include <random> // Just for testing

static std::random_device rd;           // For random maps that change
static std::default_random_engine re;   // For random maps that don't change
//static std::uniform_int_distribution<int> distr(1, 4);
static std::geometric_distribution<int> distr;

class Item;

// Must use like: Terrain::Meadow, this is to keep them getting mixed up with the color enums in display
enum class Terrain
{
    UNDISCOVERED, // Not really a terrain type, just convenient for coloring in display
    MEADOW,
    SWAMP,
    WATER,
    WALL
};

struct MapSquare
{
    bool discovered;
    Terrain terrain;
    Item* item;

    //MapSquare() = default;
    MapSquare() : discovered{false}, terrain{}, item{} {} // Just for testing!

    MapSquare(bool discovered, Terrain terrain, Item* item) 
        : discovered{discovered}, terrain{terrain}, item{item}
    {}
};

class Map
{
    int width, height;
    MapSquare** map;

public:
    Map(int width, int height);
    ~Map();

    Map(Map&& other)
        : width{other.width}, height{other.height}, map{other.map}
    {
        other.map = nullptr;
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    std::pair<int, int> getXY() const { return {width, height}; }
    const MapSquare& sq(int x, int y) const { return map[y][x]; }
    MapSquare& sq(int x, int y) { return map[y][x]; }

    // Func is a lambda/function which takes (int, int, const MapSquare&)
    template<class Func>
    void loopMap(const Func& func) const
    {
        for(int i = 0; i < height; ++i)
            for(int j = 0; j < width; ++j)
                func(j, i, map[i][j]);
    }

};