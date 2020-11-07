#pragma once

class Map;
class Camera;
class Player;

class Display
{
public:
    Display();

    void printMap(Camera& camera, Map& map);
    void printCharacter(Camera& camera, Player& player);

};