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

public:
    MapGenerator(int size, uint_fast32_t seed) 
        : size{size}, seed{seed}, re{seed}
    {}

    Map voronoi(int dim, int cells, int numLeaders);
private:
    void buildVornoiPoints(int cells, std::vector<int>& px, std::vector<int>& py);
    void assignVoronoiCells(int dim, int cells, std::vector<int>& px, std::vector<int>& py,
        std::map<std::pair<int, int>, int>& mapCells,
        std::map<int, std::set<std::pair<int, int>>>& voronoiCells,
        std::map<int, std::vector<std::pair<int, int>>>& voronoiCellsVec);

    Map buildMap(int dim, std::map<std::pair<int, int>, int>& mapCells,
        std::map<int, std::set<std::pair<int, int>>>& voronoiCells, 
        std::map<int, std::vector<int>>& lMembers);
    void setTileTypeFromGroup(Map& map, Terrain terrain, 
        std::set<std::pair<int, int>>& cellMembers, int& mapCellCount);
    
    void buildWalls(Map& map, int num, std::map<int, std::set<std::pair<int, int>>>& voronoiCells, 
        std::map<std::pair<int, int>, int>& mapCells, std::map<int, Terrain>& terrainMappings);

    void placeWallObstacles(Map& map, std::vector<bool>& isWallVerticle,
        std::map<int, std::vector<std::pair<int, int>>>& mapWallSquares);
};