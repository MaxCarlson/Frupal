#pragma once
#include <cstdint>
#include <random>
#include <map>
#include <set>

class Map;
enum class Terrain;

class MapGenerator
{
    int size;
    uint_fast32_t seed;
    std::default_random_engine re;

    std::vector<int>    px;
    std::vector<int>    py; 
    std::vector<int>    leaders;
    std::set<int>       notFilled;
    std::map<std::pair<int, int>, int>              mapCells;
    std::map<int, std::set<std::pair<int, int>>>    voronoiCells;
    std::map<int, std::vector<std::pair<int, int>>> voronoiCellsVec; 
    std::map<int, std::vector<int>>                 lMembers;

public:
    MapGenerator(int size, uint_fast32_t seed) 
        : size{size}, seed{seed}, re{seed}
    {}

    Map generate(int dim, int cells, int numLeaders);


private:
    // Build a voronoi map
    Map voronoi(int dim, int cells, int numLeaders);

    // Generate cells number of Voronoi points, coordinates of px & py
    void buildVornoiPoints(int cells);

    // Map map squares to the appropriate Voronoi cell
    void assignVoronoiCells(int dim, int cells);

    // Build various data structures required by Build Map & other functions
    void buildVoronoiHelpers(int numLeaders, int cells);

    // Assign map squares terrain based on the Voronoi leader's groups
    Map buildMap(int dim);

    // Assign all members of a Voronoi cell a particular terrain type
    void setTileTypeFromGroup(Map& map, Terrain terrain, 
        std::set<std::pair<int, int>>& cellMembers, int& mapCellCount);
    
    void buildWalls(Map& map, int num, std::map<int, Terrain>& terrainMappings);

    // Place obstacles in all walls in a way that allows all walls to be passed through
    void placeWallObstacles(Map& map, std::vector<bool>& isWallVerticle,
        std::map<int, std::vector<std::pair<int, int>>>& mapWallSquares);

    // Place all items on the map
    void placeItems(Map& map, std::map<int, Terrain>& terrainMappings);
};