#pragma once
#include <cstdint>

class Map;
class Player;
class Camera;
class Display;
class Item;

class UI
{
    int cols;
public:

    UI(int tcols)
        : cols{tcols % 2 ? 23 : 22} // This handles the rounding issues that arise from the camera
    {}

    void mainMenu(Display& display, bool& gameRunning, bool& loadMap, uint32_t& seed);
    int getSize() const { return cols; }
    void print(Display& display, const Player& player, const Camera& camera, Map& map);

private:

    void printOutline(Display& display, const Camera& camera);
    void printSelectedInfo(const Player& player, Map& map, const Camera& camera, int xOffset);
    uint32_t seedSelection(Display& display, uint32_t currentSeed);
    void printWindow(Item*);
};
