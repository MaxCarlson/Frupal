#pragma once
#include "camera.h"



// Must use like: Terrain::Meadow, this is to keep them getting mixed up with the color enums in display
enum class Terrain
{
    UNDSICOVERED,
    MEADOW,
    SWAMP,
    WATER,
    WALL
};

enum class Obstacle
{
    NONE,
    TREE,
    BOULDER,
};

struct MapSquare
{
    Terrain terrain;
    Obstacle obstacle;

    MapSquare() = default;
    MapSquare(Terrain terrain, Obstacle obstacle) 
        : terrain{terrain}, obstacle{obstacle}
    {}
};

class Map
{
    int width, height;
    MapSquare** map;

public:
    Map(int width, int height) :
        width{width}, height{height}
    {
        map = new MapSquare*[height];

        // Default map squares to undiscovered
        for(int i = 0; i < height; ++i)
            map[i] = new MapSquare[width]();
    }

    ~Map()
    {
        for(int i = 0; i < height; ++i)
            delete[] map[i];
        delete[] map;
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    template<class Func>
    void loopMap(const Func& func)
    {
        // Width and height are inverted in order here because array access is faster like this
        for(int i = 0; i < height; ++i)
            for(int j = 0; j < width; ++j)
                func(j, i, map[i][j]);
    }

};