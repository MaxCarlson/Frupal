#pragma once

class Map;
class Player;

class Movement
{
public:
    static void movePlayer(Player& player, Map& map, int x, int y);
};