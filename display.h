#pragma once
#include <string>

class Map;
class Camera;
class Player;
class UI;

class Display
{
public:
    Display();

    void printMap(const Camera& camera, const Map& map, const UI& ui);
    void printCharacter(const Camera& camera, const Player& player);
    void printUI(const Camera& camera, UI& ui, const Player& player, Map& map);
    void printCenteredText(int x1, int x2, int y, std::string text);
    int deathScreen();
};
