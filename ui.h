#pragma once

class Map;
class Player;
class Camera;
class Display;

class UI
{
public:

    static constexpr int cols = 25;


    void print(Display& display, const Player& player, const Camera& camera, Map& map);

private:

    void printOutline(Display& display, const Camera& camera);
    void printSelectedInfo(const Player& player, Map& map, const Camera& camera, int xOffset);
};