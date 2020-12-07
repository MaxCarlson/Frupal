#pragma once
#include "items/obstacle.h"

class Player;
class Map;
class UI;
class Camera; 

class Input
{

public:
    Input() = default;

    bool input(Player& player, Map& map, UI& ui, Camera& camera); 
    bool buyItem(const Camera& camera, const UI& ui); 
    bool canBreakObstacle(Player& player, Obstacle *obstacle, int obstacleCost);
};
