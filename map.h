#pragma once
#include "camera.h"
#include <random>
#include <functional>
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

// Note: This isn't in widespread use, but should be used for all future points. 
// It was stupid not to have it from the start
struct Point
{
    int x, y;
    Point() = default;
    Point(int x, int y) : x{x}, y{y} {}

    bool operator==(const Point& p) const { return x == p.x && y == p.y; }
};

namespace std
{
    template<>
    struct hash<Point>
    {
        size_t operator()(const Point& p) const
        {
            return p.x + p.y * 128;
        }
    };
}

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
    Map() = default;
    Map(int width, int height);
    ~Map();

    Map(Map&& other)
        : width{other.width}, height{other.height}, map{other.map}
    {
        other.map = nullptr;
    }

    Map& operator=(Map&& m)
    {
        width = m.width;
        height = m.height;
        map = m.map;
        m.map = nullptr;

        return *this;
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    std::pair<int, int> getXY() const { return {width, height}; }
    const MapSquare& sq(int x, int y) const { return map[x][y]; }
    const MapSquare& sq(const Point& p) const { return map[p.x][p.y]; }

    MapSquare& sq(int x, int y) { return map[x][y]; }
    MapSquare& sq(const Point& p) { return map[p.x][p.y]; }

    // Func is a lambda/function which takes (int, int, const MapSquare&)
    template<class Func>
    void loopMap(const Func& func) const
    {
        for(int j = 0; j < height; ++j)
            for(int i = 0; i < width; ++i)
                func(i, j, map[i][j]);
    }

    template<class Func>
    void loopNeighbors(Point p, const Func& func) const
    {
        bool stop = false;
        // Up
        if(p.y > 0)
            func(Point{p.x, p.y-1}, stop);

        // Right
        if(!stop && p.x < getWidth() - 1)
            func(Point{p.x+1, p.y}, stop);

        // Down
        if(!stop && p.y < getHeight() - 1)
            func(Point{p.x, p.y+1}, stop);

        // Left
        if(!stop && p.x > 0)
            func(Point{p.x-1, p.y}, stop);
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
