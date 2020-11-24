#pragma once

class Map;
class Player;
class Camera;
class Display;

class UI
{
    int cols;
public:

    UI(int tcols)
        : cols{tcols % 2 ? 23 : 22} // This handles the rounding issues that arise from the camera
    {}

    int getSize() const { return cols; }
    void print(Display& display, const Player& player, const Camera& camera, Map& map);

private:

    void printOutline(Display& display, const Camera& camera);
    void printSelectedInfo(const Player& player, Map& map, const Camera& camera, int xOffset);

};
