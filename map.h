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

    MapSquare() : discovered{true}, terrain{}, item{nullptr} {}
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
    const MapSquare& sq(int x, int y) const { return map[x][y]; }
    MapSquare& sq(int x, int y) { return map[x][y]; }

    // Func is a lambda/function which takes (int, int, const MapSquare&)
    template<class Func>
    void loopMap(const Func& func) const
    {
        for(int j = 0; j < height; ++j)
            for(int i = 0; i < width; ++i)
                func(i, j, map[i][j]);
    }

    //template<class Func>
    //void loopMap(const Func& func)
    //{
    //    for(int j = 0; j < height; ++j)
    //      for(int i = 0; i < width; ++i)
    //          func(i, j, map[i][j]);
    //}

    //template<class Func>
    //void loopMapCoords(const Func& func) const
    //{
    //    for(int j = 0; j < height; ++j)
    //      for(int i = 0; i < width; ++i)
    //          func(i, j);
    //}
};
