#pragma once
#include "items/obstacle.h"

class Player;
class Map;

class Input
{

public:
    Input() = default;

    bool input(Player& player, Map& map);
    bool buyItem();

  
private:
   

    bool canBreakObstacle(Player& player, Obstacle *obstacle, int obstacleCost);

};
