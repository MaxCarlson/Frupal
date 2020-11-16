#pragma once
#include <vector>

class Map;
struct Point;

class Pathing
{
public:
    bool playerToDiamond(const Map& map, Point player, Point diamond, 
        std::vector<Point>& requiredBoats);
private:
    std::vector<Point> aStar(const Map& map, Point player, Point diamond, 
        std::vector<Point>& requiredBoats);
    
    void findRequiredBoatLocs(const Map& map, std::vector<Point>& rqPoints, 
        const std::vector<Point>& path);
};