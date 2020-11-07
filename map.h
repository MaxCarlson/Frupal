#pragma once
#include "camera.h"

enum Obstacle
{
    NONE = 0,
    TREE,
    BOULDER,
};

enum Terrain
{
    MEADOW,
    SWAMP,
    WATER,
    WALL
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

        for(int i = 0; i < height; ++i)
            map[i] = new MapSquare[width];
    }

    ~Map()
    {
        for(int i = 0; i < height; ++i)
            delete[] map[i];
        delete[] map;
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }

};