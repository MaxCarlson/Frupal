#pragma once

class Map;
class Player;
class Camera; 
class UI; 

class Movement
{
public:
    static void movePlayer(Player& player, Map& map, UI& ui, Camera& camera, int x, int y);
};

