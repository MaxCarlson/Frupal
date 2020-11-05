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

    MapSquare map[mapHeight][mapWidth];

public:


};