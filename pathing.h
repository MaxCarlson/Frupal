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
    void aStar(const Map& map, Point player, Point diamond, 
        std::vector<Point>& requiredBoats);
};