#pragma once

class Player;
class Map;

class Input
{

public:
    Input() = default;

    bool input(Player& player, Map& map);
    bool buyItem();
    bool canBreakObstacle(Player& player, int obstacleCost);
};
