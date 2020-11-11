#pragma once
#include <cstdint>
#include <random>
#include <map>
#include <set>

class MapGenerator
{
    int size;
    uint_fast32_t seed;
    std::uniform_int_distribution<int> dist;
    std::default_random_engine re;

public:
    MapGenerator(int size, uint_fast32_t seed) 
        : size{size}, seed{seed}, dist{0, size}, re{seed}
    {}

    void voronoi(int dim, int cells, int numLeaders);
private:
    void assignVoronoiCells(int dim, int cells, std::vector<int>& px, std::vector<int>& py,
        std::map<std::pair<int, int>, int>& mapCells,
        std::map<int, std::set<std::pair<int, int>>>& voronoiCells);
};