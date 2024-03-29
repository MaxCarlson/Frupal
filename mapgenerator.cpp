#include "mapgenerator.h"
#include "map.h"
#include "items/binoculars.h"
#include "items/chest.h"
#include "items/clue.h"
#include "items/diamond.h"
#include "items/food.h"
#include "items/obstacle.h"
#include "items/ship.h"
#include "items/tool.h"
#include "items/itemloader.h"
#include "pathing.h"
#include <assert.h>
#include <algorithm>
#include <unordered_set>

MapGenerator::MapGenerator(int size, uint_fast32_t seed, ItemLoader& itemLoader) 
        : size{size}, seed{seed}, itemLoader{itemLoader}, re{seed}, px{}, py{}, 
        leaders{}, notFilled{}, mapCells{}, voronoiCells{}, 
        voronoiCellsVec{}, lMembers{}
{}

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

void MapGenerator::buildVornoiPoints(int cells)
{
    // Build Voronoi points
    std::set<std::pair<int, int>> points;
    std::uniform_int_distribution<int> dist{0, size-1};
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

void MapGenerator::assignVoronoiCells(int cells)
{
    for(int x = 0; x < size; ++x)
        for(int y = 0; y < size; ++y)
        {
            // Find the closest voronoi cell for each map cell
            int n = 0;
            for(int i = 0; i < cells; ++i)
                if(distance(px[i], py[i], x, y) < distance(px[n], py[n], x, y))
                    n = i;

            mapCells.emplace(std::pair{x, y}, n);

            auto it = voronoiCells.find(n);
            if(it == std::end(voronoiCells))
            {
                voronoiCells.emplace(n, std::set<std::pair<int, int>>{std::pair{x, y}});
                voronoiCellsVec.emplace(n, std::vector<std::pair<int, int>>{std::pair{x, y}});
            }
            else
            {
                it->second.emplace(std::pair{x, y});
                voronoiCellsVec.find(n)->second.emplace_back(std::pair{x, y});
            }
        }
    
    // Make sure every map square is assigned to a vornoi cell
    assert(mapCells.size() == size*size);
}

void MapGenerator::buildVoronoiHelpers(int numLeaders, int cells)
{
    // Choose leader points
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

    // Add leaders as members of a leader cells group
    for(int l : leaders)
        lMembers.emplace(l, std::vector<int>{l});

    // Find non-accounted for cells
    for(int i = 0; i < cells; ++i)
        if(std::find(std::begin(leaders), std::end(leaders), i) == std::end(leaders))
            notFilled.emplace(i);

    assert(notFilled.size() == cells - leaders.size());
}

Map MapGenerator::generate(int cells, int numLeaders)
{
    assert(numLeaders < cells);

    px.resize(cells);
    py.resize(cells);
    buildVornoiPoints(cells);

    // Find which map cells belong to which voronoi cells and vice versa
    assignVoronoiCells(cells);

    buildVoronoiHelpers(numLeaders, cells);

    return voronoi(cells, numLeaders);
}

Map MapGenerator::voronoi(int cells, int numLeaders)
{
    // Loop through the number of cells not assigned
    for(int d = 0; d < cells - numLeaders; ++d)
    {
        std::uniform_int_distribution<int> distL{0, numLeaders-1};
        int idx = distL(re);
        int l   = leaders[idx];
        
        // Loop through the members of each leaders group
        for(int lm : lMembers[l])
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
            // Try again
            if(lm == lMembers[l][lMembers.size() - 1])
            {
                --d;
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
    return buildMap();
}

Map MapGenerator::buildMap()
{
    Map map{size, size};

    int numLeaders = lMembers.size();

    std::vector<std::pair<int, std::vector<int>>> lMemVec;
    for(auto& lm : lMembers)
        lMemVec.emplace_back(lm.first, lm.second);

    // Sort the leader groups based on their number of cells (TODO: Maybe sort based on total map tile count)
    std::sort(std::begin(lMemVec), std::end(lMemVec), [&](auto& p1, auto& p2) {
        return p1.second.size() < p2.second.size();
    });

    // Choose number of cells for each terrain type
    int numWater    = numLeaders / 3;
    int numSwamp    = numWater / 4 + numWater;

    // Place the terrain
    int i = 0;
    int mapSqCount = 0;
    for(auto& lm : lMemVec)
    {
        for(auto& cell : lm.second)
        {
            Terrain type;
            auto& cellMembers = voronoiCellsVec.find(cell)->second;
            if(i < numWater)
                type = Terrain::WATER;
            else if (i < numSwamp)
                type = Terrain::SWAMP;
            else
                type = Terrain::MEADOW;

            terrainMappings.emplace(cell, type);
            setTileTypeFromGroup(map, type, cellMembers, mapSqCount);
        }

        ++i;
    }

    assert(mapSqCount == size*size);

    buildHouses(map, 6, 16, 5, 10);
    buildWalls(map, 20);
    placeHouseObstacles(map); // Must come after building walls
    placeItems(map);

    return map;
}

void MapGenerator::setTileTypeFromGroup(Map& map, Terrain terrain, 
    std::vector<std::pair<int, int>>& cellMembers, int& mapSqCount)
{
    for(auto [x, y] : cellMembers)
    {
        ++mapSqCount;
        map.sq(x, y).terrain = terrain;
    }
}

void MapGenerator::buildWalls(Map& map, int num)
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
        Terrain t = terrainMappings.find(cell)->second;
        if(t == Terrain::WATER)
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

        // Pick map cell out of the Voronoi cell to start in
        auto vMapCells = voronoiCells.find(cell)->second;
        auto [x, y] = *std::begin(vMapCells);

        // Don't build walls on map edge
        if(x < 1 || y < 1 || x >= map.getWidth() - 1 || y >= map.getHeight() - 1)
            goto TRY_AGAIN;

        // Determine and store wall dir
        isWallVerticle.emplace_back(xDir ? false : true);

        // Add root wall square
        auto [mapWallIt, done] = mapWallSquares.emplace(wall, std::vector<std::pair<int, int>>{std::pair{x, y}});

        // Expand wall from root
        for(;;)
        {
            x += xDir;
            y += yDir;

            if(x <= 0 || y <= 0 || x > map.getWidth() - 2 || y > map.getHeight() - 2)
                break;

            MapSquare& sq = map.sq(x, y);
            if(sq.terrain == Terrain::WATER || sq.terrain == Terrain::WALL)
                break;

            // Don't place walls with their length==1 side next to 
            // un-walkable terrain (except end squares)
            if(isWallVerticle.back() && !(validAdjacentObstacleSq(map, x+1, y) 
                && validAdjacentObstacleSq(map, x-1, y)))
                break;

            else if(!(validAdjacentObstacleSq(map, x, y+1) && validAdjacentObstacleSq(map, x, y-1)))
                break;

            sq.terrain = Terrain::WALL;
            mapWallIt->second.emplace_back(std::pair{x, y});
        }

        if(mapWallIt->second.size() <= 1)
        {
            mapWallSquares.erase(wall);
            isWallVerticle.erase(std::end(isWallVerticle) - 1);
            goto TRY_AGAIN;
        }

        map.sq(x, y).terrain = Terrain::WALL;
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
            if((isWallVerticle[wallId] && validAdjacentObstacleSq(map, x+1, y) 
                && validAdjacentObstacleSq(map, x-1, y)))
                validObstacleSqs.emplace_back(std::pair{x, y});

            else if(validAdjacentObstacleSq(map, x, y+1) && validAdjacentObstacleSq(map, x, y-1))
                validObstacleSqs.emplace_back(std::pair{x, y});
        }

        std::uniform_int_distribution<int> obSqDist{0, static_cast<int>(validObstacleSqs.size()) - 1};
        int obIdx = obSqDist(re);
        auto [x, y] = validObstacleSqs[obIdx];

        // TODO: Pick random obstacle here
        map.sq(x, y).item = new Obstacle{itemLoader.getObstacle(re)};
        
        // Refill map terrain where obstacle was placed (and the wall terrain was removed)
        if(isWallVerticle[wallId])
            map.sq(x, y).terrain = map.sq(x+1, y).terrain;
        else
            map.sq(x, y).terrain = map.sq(x, y-1).terrain;

    }
}

bool MapGenerator::validAdjacentObstacleSq(const Map& map, int x, int y) const
{
    if(x < 0 || y < 0 || x > map.getWidth() - 1 || y > map.getHeight() - 1)
        return false;
    const MapSquare& sq = map.sq(x, y);
    Terrain t = sq.terrain;
    return t != Terrain::WATER && t != Terrain::WALL && !sq.item;
}

std::pair<int, int> MapGenerator::moveSqDir(int x, int y, int dir, int len) const
{
    // Start at one of the corners of the previous corners, move diagonally in dir
    // and start iterating up the side clockwise
    switch(dir)
    {
        case NE:
        return std::pair{x + len, y - len};
        case SE:
        return std::pair{x + len, y + len};
        case SW:
        return std::pair{x - len, y + len};
        case NW:
        return std::pair{x - len, y - len};
    }
    return std::pair{0,0};
}

bool MapGenerator::checkHouseSide(int x, int y, int dir, int length, 
    const Map& map, std::vector<std::pair<int, int>>& corners)
{
    int xMod, yMod;
    switch(dir)
    {
        case NE:
        xMod = 0;  yMod = 1;  break;
        case SE:
        xMod = -1; yMod = 0;  break;
        case SW:
        xMod = 0;  yMod = -1; break;
        case NW:
        xMod = 1;  yMod = 0;  break;
    }

    auto [xc, yc] = moveSqDir(x, y, dir, 1);

    auto isValid = [&](int x, int y)
    {
        if(x < 0 || y < 0 || x >= map.getWidth() || y >= map.getWidth())
            return false;
        return map.sq(x, y).terrain == Terrain::MEADOW;
    };

    for(int i = 0; i < length; ++i)
    {
        if(!isValid(xc, yc))
            return false;
        xc += xMod;
        yc += yMod;
    }
    return true;
}

std::tuple<int, int, int> MapGenerator::findHouseLocation(const Map& map, int minSide, 
    int maxSide, std::vector<std::pair<int, int>>& corners)
{
    std::uniform_int_distribution<int> distCell{0, static_cast<int>(voronoiCells.size()) - 1};

    // TODO: This is ineffecient and could be made more so by 
    // storing a map of all cells by terrain types
    // Select random Meadow cell not occupied by a house
    RESTART:
    int mcell = -1;
    for(;;)
    {
        mcell = distCell(re);
        if(terrainMappings.find(mcell)->second == Terrain::MEADOW
            && houseCells.find(mcell) == std::end(houseCells))
            break;
    }

    // Select middle cell point of cell
    auto find = voronoiCellsVec.find(mcell);
    auto [x, y] = find->second[find->second.size() / 2];

    if(x < minSide - 1 || y < minSide - 1 || x >= map.getWidth() - minSide - 1 
        || x >= map.getHeight() - minSide - 1 )
        goto RESTART;

    // Find the largest Rectangle around the choosen point
    std::fill(std::begin(corners), std::end(corners), std::pair{x, y});

    int len;
    for(len = 1; len <= maxSide - 1; len += 2)
    {
        // Check if all sides are valid
        int cornerDistance  = 1;
        bool invalidSide    = false;
        for(int d = 0; d < 4; ++d)
            if(!checkHouseSide(x, y, d, cornerDistance++, map, corners))
            {
                invalidSide     = true;
                break;
            }

        if(invalidSide)
        {
            len -= 1;
            break;
        }

        houseCells.emplace(mcell);
        
        // Move the corners outwards
        int dir = 0;
        for(auto& [xc, yc] : corners)
        {
            auto [nxc, nyc] = moveSqDir(xc, yc, dir++, 1);
            xc = nxc;
            yc = nyc;
            houseCells.emplace(mapCells.find(std::pair{xc, yc})->second);
        }
    }
    return std::tuple{x, y, len-1};
}

void MapGenerator::buildHouses(Map& map, int min, int max, int minSide, int maxSide)
{

    std::vector<std::pair<int, int>> corners(4);
    std::uniform_int_distribution<int> dist{min, max};
    int numHouses = dist(re);

    for(int i = 0; i < numHouses; ++i)
    {
        auto [x, y, len] = findHouseLocation(map, minSide, maxSide, corners);        

        // Try again if our minimum side length isn't met
        if(len < minSide)
        {
            --i;
            continue;
        }

        std::uniform_int_distribution<int> distS{minSide, len};
        int halfXLen = (distS(re) - 1) / 2;
        int halfYLen = (distS(re) - 1) / 2;

        auto buildWalls = [&](int xMod, int yMod, int len)
        {
            int yts = y + yMod;
            int xts = x + xMod;

            // Handle root wall coordinate
            map.sq(xts, yts).terrain = Terrain::WALL;
            auto [it, done] = houseWallCoords.emplace(i, 
                std::vector<std::pair<int, int>>{std::pair{xts, yts}});

            // 
            for(int i = 1; i < len+1; ++i)
            {
                int xWall0 = xts + i * (yMod != 0);
                int xWall1 = xts - i * (yMod != 0);
                int yWall0 = yts + i * (xMod != 0);
                int yWall1 = yts - i * (xMod != 0);
                map.sq(xWall0, yWall0).terrain = Terrain::WALL;
                map.sq(xWall1, yWall1).terrain = Terrain::WALL;
                
                it->second.emplace_back(std::pair{xWall0, yWall0});
                it->second.emplace_back(std::pair{xWall1, yWall1});
            }
        };

        // Build top and bottom of house
        buildWalls(0,  halfYLen, halfXLen);
        buildWalls(0, -halfYLen, halfXLen);
        // Build left and right sides
        buildWalls(halfXLen,  0, halfYLen);
        buildWalls(-halfXLen, 0, halfYLen);

        // Build the coordiantes of houses
        int topLeftX = x - halfXLen + 1;
        int topLeftY = y - halfYLen + 1;

        for(int i = topLeftX; i < topLeftX + halfXLen * 2 - 1; ++i)
        {
            auto& it = houseCoords.emplace_back(std::vector<std::pair<int, int>>{});
            for(int j = topLeftY; j < topLeftY + halfYLen * 2 - 1; ++j)
                it.emplace_back(std::pair{i, j});
        }
    }
}

void MapGenerator::placeHouseObstacles(Map& map)
{
    for(const auto& [wallId, coordsVec] : houseWallCoords)
    {
        std::uniform_int_distribution<int> dist{0, static_cast<int>(coordsVec.size()) - 1};
        for(int i = 0; i < static_cast<int>(coordsVec.size()); ++i)
        {
            auto [x, y] = coordsVec[dist(re)];
            int validSqs = 0;
            validSqs += validAdjacentObstacleSq(map, x, y-1) ? 1 << 0 : 0;
            validSqs += validAdjacentObstacleSq(map, x, y+1) ? 1 << 1 : 0;
            validSqs += validAdjacentObstacleSq(map, x-1, y) ? 1 << 2 : 0;
            validSqs += validAdjacentObstacleSq(map, x+1, y) ? 1 << 3 : 0;

            // Neither a pair of squares above/below, nor left/right are free
            // Don't place obstacle here
            if(!(validSqs == 3 || validSqs == 12))
                continue;

            MapSquare& sq   = map.sq(x, y);
            //sq.item         = new Obstacle{"ObstacleTest", "ObstacleType", 10};
            sq.item = new Obstacle{itemLoader.getObstacle(re)};
            sq.terrain      = validSqs == 3 ? map.sq(x, y+1).terrain : map.sq(x+1, y).terrain;
            if(sq.terrain == Terrain::WALL)
                sq.terrain  = validSqs == 3 ? map.sq(x, y-1).terrain : map.sq(x-1, y).terrain;
            break;
        }
    }
}

template<class ItemType>
void scatterItems(Map& map, std::map<int, std::set<std::pair<int, int>>>& voronoiCells, 
    std::map<int, std::vector<std::pair<int, int>>>& voronoiCellsVec,
    std::map<int, Terrain>& terrainMappings, const std::map<Terrain, float>& chanceInTerrain, 
    const std::set<Terrain>& types, const ItemLoader& itemLoader, std::default_random_engine& re)
{
    std::uniform_real_distribution<float> dist{0.f, 1.f};

    // Note, in order to get the parameter pack args into the llambda, we need to use the ugly hack in the [] below
    auto addItemToRandomSq = [&]
        (auto& distM, const auto& coords)
    {
        for(int i = 0; i < static_cast<int>(coords.size()); ++i)
        {
            int mapCell = distM(re);
            auto [x, y] = coords[mapCell];
            MapSquare& sq = map.sq(x, y);
            if(types.find(sq.terrain) == std::end(types) || sq.item)
                continue;

            // TODO: We're going to have to grab the Items arguments randomly from a loaded text file
            //sq.item = new ItemType{itemLoader.getItem<ItemType>(re)};
            sq.item = new ItemType{itemLoader.getItem<ItemType>(re,std::pair<int,int> {x, y})};

            return true;
        }
        return false;
    };

    for(const auto& [cId, coords] : voronoiCellsVec)
    {
        Terrain ttype = terrainMappings.find(cId)->second;
        // Don't add items to the wrong terrains
        if(types.find(ttype) == std::end(types))
            continue;

        // While cell chance is above 1.0, place an item in a cell
        float cellChance = chanceInTerrain.find(ttype)->second; 
        std::uniform_int_distribution<int> distM{0, static_cast<int>(coords.size()) - 1};
        while(cellChance >= 1.f)
        {
            // Add item to random map square inside the voronoi cell
            if(addItemToRandomSq(distM, coords))
                cellChance -= 1.f;
            else
                break;
        }
        // No free map squares in this cell
        if(cellChance >= 1.f)
            continue;

        // Skip this cell if we don't pass the check
        float rv = dist(re);
        if(rv >= cellChance)
            continue;

        addItemToRandomSq(distM, coords);
    }
}

template<class ItemType>
void scatterItemsInHouses(float chancePerCell, Map& map, const ItemLoader& itemLoader,
    std::vector<std::vector<std::pair<int, int>>>& houseCoords, std::default_random_engine& re)
{
    std::uniform_real_distribution<float> dist{};
    for(const auto& coordsVec : houseCoords)
        for(const auto& [x, y] : coordsVec)
        {
            if(chancePerCell < dist(re))
                continue;

            MapSquare& sq = map.sq(x, y);
            if(sq.terrain == Terrain::WATER || sq.item)
                continue;

            sq.item = new ItemType{itemLoader.getItem<ItemType>(re,std::pair<int,int> {x, y})};
        }
}

void MapGenerator::placeItems(Map& map)
{
    std::set<Terrain> mostItemTerrainTypes              = {Terrain::MEADOW, Terrain::SWAMP};
    std::map<Terrain, float> foodChanceInTerrain        = {{Terrain::MEADOW, 0.5f},  {Terrain::SWAMP, 1.f}}; 
    std::map<Terrain, float> toolChanceInTerrain        = {{Terrain::MEADOW, 0.2f},  {Terrain::SWAMP, 0.2f}}; 
    std::map<Terrain, float> treasureChanceInTerrain    = {{Terrain::MEADOW, 0.1f},  {Terrain::SWAMP, 0.2f}}; 
    std::map<Terrain, float> binocularChanceInTerrain   = {{Terrain::MEADOW, 0.01f}, {Terrain::SWAMP, 0.02f}}; 
    std::map<Terrain, float> clueChanceInTerrain        = {{Terrain::MEADOW, 0.005f}, {Terrain::SWAMP, 0.008f}}; 

    scatterItems<Food>(map, voronoiCells, voronoiCellsVec, 
        terrainMappings, foodChanceInTerrain, mostItemTerrainTypes, itemLoader, re);

    scatterItems<Chest>(map, voronoiCells, voronoiCellsVec, 
        terrainMappings, treasureChanceInTerrain, mostItemTerrainTypes, itemLoader, re);

    scatterItems<Tool>(map, voronoiCells, voronoiCellsVec, 
        terrainMappings, toolChanceInTerrain, mostItemTerrainTypes, itemLoader, re);

    scatterItems<Binoculars>(map, voronoiCells, voronoiCellsVec, 
        terrainMappings, binocularChanceInTerrain, mostItemTerrainTypes, itemLoader, re);

    
    placeItemsInHouses(map);

    std::vector<Point> reqBoats;
    placePlayerAndDiamod(map, reqBoats);
    placeBoats(map, reqBoats, 0.4f);

    scatterItems<Clue>(map, voronoiCells, voronoiCellsVec, 
        terrainMappings, clueChanceInTerrain, mostItemTerrainTypes, itemLoader, re);
}

void MapGenerator::placeItemsInHouses(Map& map)
{
    // Chance per house cell 
    // for each item type  
    float chestChance   = 0.09;
    float clueChance    = 0.03;
    float foodChance    = 0.03;
    float toolChance    = 0.03;

    scatterItemsInHouses<Chest>(chestChance, map, itemLoader, houseCoords, re);
    scatterItemsInHouses<Clue>(  clueChance, map, itemLoader, houseCoords, re);
    scatterItemsInHouses<Food>(  foodChance, map, itemLoader, houseCoords, re);
    scatterItemsInHouses<Tool>(  toolChance, map, itemLoader, houseCoords, re);
}

std::tuple<int, int, int, int, int> getRandomCornerCoords(const Map& map, int notThisCorner=-1)
{
    int xMin, xMax;
    int yMin, yMax;
    int corner = notThisCorner;
    std::uniform_int_distribution<int> distCorner{0, 3};

    while(corner == notThisCorner)
        corner = distCorner(re);

    switch(corner)
    {
        case MapGenerator::Corner::NE:
        xMin = map.getWidth() / 2 - 1; xMax = map.getWidth() - 1;
        yMin = 0; yMax = map.getHeight() / 2 - 1;
        break;
        case MapGenerator::Corner::SE:
        xMin = map.getWidth()  / 2 - 1; xMax = map.getWidth()  - 1;
        yMin = map.getHeight() / 2 - 1; yMax = map.getHeight() - 1;
        break;
        case MapGenerator::Corner::SW:
        xMin = 0; xMax = map.getWidth() / 2 - 1;
        yMin = map.getHeight() / 2 - 1; yMax = map.getHeight() - 1;
        break;
        case MapGenerator::Corner::NW:
        xMin = 0; xMax = map.getWidth() / 2 - 1;
        yMin = 0; yMax = map.getHeight() / 2 - 1;
        break;
    }  

    return std::make_tuple(xMin, xMax, yMin, yMax, corner);
}

template<class RandomEngine, class Func>
void placeInCorner(Map& map, RandomEngine& re, int xMin, int xMax, 
    int yMin, int yMax, Func&& func)
{
    std::uniform_int_distribution<int> distX{xMin, xMax};
    std::uniform_int_distribution<int> distY{yMin, yMax};

    for(;;)
    {
        int x = distX(re);
        int y = distY(re);

        MapSquare& sq = map.sq(x, y);

        if(sq.terrain == Terrain::WALL || sq.terrain == Terrain::WATER
            || sq.item)
            continue;

        if(func(x, y, sq))
            return;
    }
}

void MapGenerator::placePlayerAndDiamod(Map& map, std::vector<Point>& reqBoats)
{
    Point diamondPoint;
    Corner diamondCorner;
    {
        auto [xMin, xMax, yMin, yMax, corner] = getRandomCornerCoords(map);

        placeInCorner(map, re, xMin, xMax, yMin, yMax, [&](int x, int y, MapSquare& sq)
        {
            diamondPoint = Point{x, y};
            diamondX = x;
            diamondY = y;
            sq.item = new Diamond{"Diamond"};
            return true;
        });
        diamondCorner = static_cast<Corner>(corner);

    }

    {
        // We never place the diamond in the same corner as the player
        auto [xMin, xMax, yMin, yMax, corner] = getRandomCornerCoords(map, diamondCorner);
        Pathing pathing;

        placeInCorner(map, re, xMin, xMax, yMin, yMax, [&](int x, int y, MapSquare& sq)
        {
            // TODO: Need to check if game is completeable
            bool success = pathing.playerToDiamond(map, Point{x, y}, diamondPoint, reqBoats);
            playerCoords = std::pair{x, y};
            if(success)
                return true;
            return false;
        });
    }
}

void MapGenerator::placeBoats(Map& map, 
    const std::vector<Point>& reqBoats, float chancePerCell)
{
    // Place boats that might be required
    std::set<int> shipCells;
    for(Point p : reqBoats)
    {
        // Add ships
        MapSquare& sq = map.sq(p);
        assert(sq.terrain == Terrain::WATER);
        sq.item = new Ship{"Ship"};

        // Add the cell the ship occupies to the set of ship cells
        shipCells.emplace(mapCells.find(std::pair{p.x, p.y})->second);
    }

    // Place boats randomly
    // TODO: Create a map opposite of terrainMappings, 
    // map terrain types to a vector of cells of that terrain type
    std::uniform_real_distribution<float> dist;
    for(auto [cellId, terrain] : terrainMappings)
    {
        if(terrain != Terrain::WATER)
            continue;

        float r = dist(re);
        if(r < chancePerCell)
            continue;

        // Don't place boats it cells that already contain them
        auto [it, done] = shipCells.emplace(cellId);
        if(!done)
            continue;

        // Find squares ships can be placed on in the cell
        std::vector<Point> validShipSqs;
        auto mapSqs = voronoiCells.find(cellId)->second;

        for(auto [x, y] : mapSqs)
        {
            map.loopNeighbors(Point{x, y}, [&](Point np, bool& stop)
            {
                const MapSquare& sq = map.sq(np);
                if(!(sq.terrain == Terrain::MEADOW || sq.terrain == Terrain::SWAMP)
                    && !sq.item)
                    return;

                stop = true;
                validShipSqs.emplace_back(Point{x, y});
            });
        }

        if(validShipSqs.empty())
            continue;

        std::uniform_int_distribution<int> distSq{0, static_cast<int>(validShipSqs.size()) - 1};
        int sqIdx = distSq(re);
        map.sq(validShipSqs[sqIdx]).item = new Ship{"Ship"};
    }
}
