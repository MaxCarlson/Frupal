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
    enum {NE, SE, SW, NW};

    std::vector<int>    px;
    std::vector<int>    py; 
    std::vector<int>    leaders;
    std::set<int>       notFilled;
    std::set<int>       houseCells;
    std::map<int, Terrain>                          terrainMappings;
    std::map<std::pair<int, int>, int>              mapCells;
    std::map<int, std::set<std::pair<int, int>>>    voronoiCells;
    std::map<int, std::vector<std::pair<int, int>>> voronoiCellsVec; 
    std::map<int, std::vector<int>>                 lMembers;
    std::map<int, std::vector<std::pair<int, int>>> houseWallCoords;

public:
    MapGenerator(int size, uint_fast32_t seed) 
        : size{size}, seed{seed}, re{seed}, px{}, py{}, 
        leaders{}, notFilled{}, mapCells{}, voronoiCells{}, 
        voronoiCellsVec{}, lMembers{}
    {}

    Map generate(int cells, int numLeaders);

private:
    // Build a voronoi map
    Map voronoi(int cells, int numLeaders);

    // Generate cells number of Voronoi points, coordinates of px & py
    void buildVornoiPoints(int cells);

    // Map map squares to the appropriate Voronoi cell
    void assignVoronoiCells(int cells);

    // Build various data structures required by Build Map & other functions
    void buildVoronoiHelpers(int numLeaders, int cells);

    // Assign map squares terrain based on the Voronoi leader's groups
    Map buildMap();

    // Assign all members of a Voronoi cell a particular terrain type
    void setTileTypeFromGroup(Map& map, Terrain terrain, 
        std::vector<std::pair<int, int>>& cellMembers, int& mapCellCount);
    
    void buildWalls(Map& map, int num);

    // Place obstacles in all walls in a way that allows all walls to be passed through
    void placeWallObstacles(Map& map, std::vector<bool>& isWallVerticle,
        std::map<int, std::vector<std::pair<int, int>>>& mapWallSquares);
    bool validAdjacentObstacleSq(const Map& map, int x, int y) const;

    std::pair<int, int> moveSqDir(int x, int y, int dir, int len) const;
    bool checkHouseSide(int x, int y, int dir, int length, 
        const Map& map, std::vector<std::pair<int, int>>& corners);
    std::tuple<int, int, int> findHouseLocation(const Map& map, int maxSide, int minSide,
        std::vector<std::pair<int, int>>& corners);
    void buildHouses(Map& map, int min, int max, int minSide, int maxSide);
    void placeHouseObstacles(Map& map);

    // Place all items on the map
    void placeItems(Map& map);
};