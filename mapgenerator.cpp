#include "mapgenerator.h"
#include "map.h"
#include "items/obstacle.h"
#include <assert.h>
#include <algorithm>

double distance(int x1, int y1, int x2, int y2)
{
    //return ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)); // Euclidean Distance
    return std::abs(x1 - x2) + std::abs(y1 - y2); // Manhattan Distance
}

// TODO: Adjacecy matrix? Or build adjacency info for voronoi cells first?
bool isAdjacent(int x1, int y1, int x2, int y2)
{
    int diffX = (x1 - x2) * (x1 - x2);
    int diffY = (y1 - y2) * (y1 - y2);

    return diffX <= 1 && diffY <= 1;
}

void MapGenerator::buildVornoiPoints(int cells, std::vector<int>& px, std::vector<int>& py)
{
    // Build Voronoi points
    std::set<std::pair<int, int>> points;
    std::uniform_int_distribution<int> dist{0, size};
    for(int i = 0; i < cells; ++i)
    {
        int x = dist(re);
        int y = dist(re);

        // Don't double up on Voronoi cell points
        if(points.find(std::pair{x, y}) != std::end(points))
        {
            --i;
            continue;
        }

        px[i] = x;
        py[i] = y;
        points.emplace(std::pair{x, y});
    }
}

void MapGenerator::assignVoronoiCells(int dim, int cells, std::vector<int>& px, std::vector<int>& py, 
    std::map<std::pair<int, int>, int>& mapCells,
    std::map<int, std::set<std::pair<int, int>>>& voronoiCells,
    std::map<int, std::vector<std::pair<int, int>>>& voronoiCellsVec)
{
    for(int x = 0; x < dim; ++x)
        for(int y = 0; y < dim; ++y)
        {
            // Find the closest voronoi cell for each map cell
            int n = 0;
            for(int i = 0; i < cells; ++i)
                if(distance(px[i], py[i], x, y) < distance(px[n], py[n], x, y))
                    n = i;

            mapCells.emplace(std::pair{x, y}, n);

            auto it = voronoiCells.find(n);
            if(it == std::end(voronoiCells))
                voronoiCells.emplace(n, std::set<std::pair<int, int>>{std::pair{x, y}});
            else
                it->second.emplace(std::pair{x, y});
        }
    
    // Make sure every map square is assigned to a vornoi cell
    assert(mapCells.size() == dim*dim);
}

Map MapGenerator::voronoi(int dim, int cells, int numLeaders)
{
    assert(numLeaders < cells);

    std::vector<int> px(cells);
    std::vector<int> py(cells);
    buildVornoiPoints(cells, px, py);

    // Find which map cells belong to which voronoi cells and vice versa
    std::map<std::pair<int, int>, int> mapCells;
    std::map<int, std::set<std::pair<int, int>>> voronoiCells;
    std::map<int, std::vector<std::pair<int, int>>> voronoiCellsVec;

    assignVoronoiCells(dim, cells, px, py, mapCells, voronoiCells, voronoiCellsVec);

    // Choose leader points
    std::vector<int> leaders;
    std::uniform_int_distribution<int> distC{0, cells-1};
    for(int i = 0; i < numLeaders; ++i)
    {
        // Don't duplicate leaders (unlikely, but possible)
        int n = distC(re);
        if(std::find(std::begin(leaders), std::end(leaders), n) == std::end(leaders))
            leaders.emplace_back(n);
        else
            --i;
    }

    std::set<int> notFilled;
    std::map<int, std::vector<int>> lMembers;

    // Add leaders as members of a leader cells group
    for(int l : leaders)
        lMembers.emplace(l, std::vector<int>{l});

    // Find non-accounted for cells
    for(int i = 0; i < cells; ++i)
        if(std::find(std::begin(leaders), std::end(leaders), i) == std::end(leaders))
            notFilled.emplace(i);

    assert(notFilled.size() == cells - leaders.size());

    // Loop through the number of cells not assigned
    for(int d = 0; d < cells - numLeaders; ++d)
    {
        std::uniform_int_distribution<int> distL{0, numLeaders-1};
        int idx = distL(re);
        int l   = leaders[idx];
        
        // Loop through the members of each leaders group
        for(auto lm : lMembers[l])
        {
            int nfId = 0;
            for(int nf : notFilled)
            {
                nfId = nf;
                // Loop though the map cells which belong to the unfilled voronoi cell
                // and find the first adjacent cell that is not filled
                auto findNf = voronoiCells.find(nf);
                auto findLm = voronoiCells.find(lm);
                for(auto [xf, yf] : findNf->second)
                    for(auto [xm, ym] : findLm->second)
                        if(isAdjacent(xf, yf, xm, ym))
                            goto FOUND;

            }
            // We didn't find any adjacent voronoi cells to this leader's groups cells
            // Close the leader's group if it is completely blocked off by filled cells
            // Remove the leader from the list, and decrement the count of leaders
            if(lm == lMembers[l][lMembers.size() - 1])
            {
                --d;
                --numLeaders;
                auto find = std::find(std::begin(leaders), std::end(leaders), l);
                leaders.erase(find);
                break;
            }
            
            FOUND:
            auto find = lMembers.find(l);
            find->second.emplace_back(nfId);
            notFilled.erase(nfId);
            break;
        }
    }
    //*/
    assert(notFilled.empty());
    return buildMap(dim, mapCells, voronoiCells, lMembers);
}

template<class ItemType>
void scatterItems(Map& map, std::map<int, std::set<std::pair<int, int>>>& voronoiCells, 
    std::map<int, Terrain>& terrainMappings, const std::map<Terrain, float>& chanceInTerrain, std::set<Terrain> types)
{
    std::uniform_real_distribution<float> dist{0.f, 1.f};

    for(const auto& [cId, coords] : voronoiCells)
    {
        Terrain ttype       = terrainMappings.find(cId)->second;
        float cellChance    = chanceInTerrain.find(ttype)->second; 


        std::uniform_int_distribution<int> distM{0, static_cast<int>(coords.size()) - 1};
        while(cellChance >= 1.f)
        {

        }
    }
}

Map MapGenerator::buildMap(int dim, std::map<std::pair<int, int>, int>& mapCells,
    std::map<int, std::set<std::pair<int, int>>>& voronoiCells, 
    std::map<int, std::vector<int>>& lMembers)
{
    Map map{dim, dim};

    int numLeaders = lMembers.size();

    std::vector<std::pair<int, std::vector<int>>> lMemVec;
    for(auto& lm : lMembers)
        lMemVec.emplace_back(lm.first, lm.second);

    // Sort the leader groups based on their number of cells (TODO: Maybe sort based on total map tile count)
    std::sort(std::begin(lMemVec), std::end(lMemVec), [&](auto& p1, auto& p2) {
        return p1.second.size() < p2.second.size();
    });

    int numWater    = numLeaders / 3;
    int numSwap     = numWater / 4 + numWater;

    std::map<int, Terrain> terrainMappings;

    int i = 0;
    int mapCellCount = 0;
    for(auto& lm : lMemVec)
    {
        for(auto& cell : lm.second)
        {
            Terrain type;
            auto& cellMembers = voronoiCells.find(cell)->second;
            if(i < numWater)
                type = Terrain::WATER;
            else if (i < numSwap)
                type = Terrain::SWAMP;
            else
                type = Terrain::MEADOW;

            terrainMappings.emplace(cell, type);
            setTileTypeFromGroup(map, type, cellMembers, mapCellCount);
        }

        ++i;
    }

    assert(mapCellCount == dim*dim);


    buildWalls(map, 20, voronoiCells, mapCells, terrainMappings);


    return map;
}

void MapGenerator::setTileTypeFromGroup(Map& map, Terrain terrain, 
    std::set<std::pair<int, int>>& cellMembers, int& mapCellCount)
{
    for(auto [x, y] : cellMembers)
    {
        ++mapCellCount;
        map.sq(x, y).terrain = terrain;
    }
}

void MapGenerator::buildWalls(Map& map, int num, std::map<int, std::set<std::pair<int, int>>>& voronoiCells, 
    std::map<std::pair<int, int>, int>& mapCells, std::map<int, Terrain>& terrainMappings)
{

    auto getDirMods = [](int dir)
    {
        switch(dir)
        {
            case 0: return std::pair{0, -1}; // Up
            case 1: return std::pair{0,  1}; // Down
            case 2: return std::pair{-1, 0}; // Left
            case 3: return std::pair{1,  0}; // Right
        }
        return std::pair{0,0};
    };

    std::set<int> wallCells;
    auto isValidCell = [&](int cell)
    {
        // Don't add multiple (different) walls in the same cell
        auto [it, done] = wallCells.emplace(cell);
        if(!done)
            return false;
        
        // No walls on water
        if(terrainMappings.find(cell)->second == Terrain::WATER)
            return false;
        return true;
    };

    
    // Build walls
    std::vector<bool> isWallVerticle;
    std::map<int, std::vector<std::pair<int, int>>> mapWallSquares;
    std::uniform_int_distribution<int> distDir{0, 3};
    std::uniform_int_distribution<int> distCell{0, static_cast<int>(voronoiCells.size()) - 1};

    for(int wall = 0; wall < num; ++wall)
    {
        TRY_AGAIN:
        int cell = distCell(re);

        if(!isValidCell(cell))
            goto TRY_AGAIN;

        // Pick wall direction
        int dir = distDir(re);
        auto [xDir, yDir] = getDirMods(dir);

        // Determine and store wall dir
        isWallVerticle.emplace_back(xDir ? false : true);

        // Pick map cell out of the Voronoi cell to start in
        auto vMapCells = voronoiCells.find(cell)->second;
        auto [x, y] = *std::begin(vMapCells);

        map.sq(x, y).terrain = Terrain::WALL;
        auto [mapWallIt, done] = mapWallSquares.emplace(wall, std::vector<std::pair<int, int>>{std::pair{x, y}});

        // Expand wall from root
        for(;;)
        {
            x += xDir;
            y += yDir;

            if(x < 0 || y < 0 || x > map.getWidth() - 1 || y > map.getHeight() - 1)
                break;

            // We're moving into a new cell, handle it
            int newCell = mapCells.find(std::pair{x, y})->second;
            if(newCell != cell && !isValidCell(newCell))
                break;

            cell = newCell;
            map.sq(x, y).terrain = Terrain::WALL;
            mapWallIt->second.emplace_back(std::pair{x, y});
        }
    }

    placeWallObstacles(map, isWallVerticle, mapWallSquares);
}

void MapGenerator::placeWallObstacles(Map& map, std::vector<bool>& isWallVerticle,
    std::map<int, std::vector<std::pair<int, int>>>& mapWallSquares)
{

    // Loop through each wall
    for(auto& [wallId, wallSquares] : mapWallSquares)
    {

        // Find wall squares that don't border impassible terrain
        std::vector<std::pair<int, int>> validObstacleSqs;
        for(auto& [x, y] : wallSquares)
        {
            auto isValidSq = [&](int x, int y) {
                if(x < 0 || y < 0 || x > map.getWidth() - 1 || y > map.getHeight() - 1)
                    return false;
                return map.sq(x, y).terrain != Terrain::WATER;
            };

            if((isWallVerticle[wallId] && isValidSq(x+1, y) && isValidSq(x-1, y)))
                validObstacleSqs.emplace_back(std::pair{x, y});
            else if(isValidSq(x, y+1) && isValidSq(x, y-1))
                validObstacleSqs.emplace_back(std::pair{x, y});
        }

        std::uniform_int_distribution<int> obSqDist{0, static_cast<int>(validObstacleSqs.size()) - 1};
        int obIdx = obSqDist(re);
        auto [x, y] = validObstacleSqs[obIdx];


        // TODO: Pick random obstacle here
        map.sq(x, y).item = new Obstacle{"test", 10};
        
        // Refill map terrain where obstacle was placed (and the wall terrain was removed)
        if(isWallVerticle[wallId])
            map.sq(x, y).terrain = map.sq(x+1, y).terrain;
        else
            map.sq(x, y).terrain = map.sq(x, y-1).terrain;
    }
}

