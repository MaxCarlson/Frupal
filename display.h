#pragma once

class Map;
class Camera;
class Player;
class UI;

class Display
{
public:
    Display();

    void printMap(const Camera& camera, const Map& map);
    void printCharacter(const Camera& camera, const Player& player);
    void printUI(const Camera& camera, const UI& ui);
};