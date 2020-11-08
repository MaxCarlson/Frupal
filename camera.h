#pragma once
#include <utility>

static constexpr int mapWidth   = 128;
static constexpr int mapHeight  = 128;

class Map;
class Player;
class UI;

class Camera
{
    int cols, lines;
    int offsetX, offsetY;
public:
    Camera(int cols, int lines) :
        cols{cols}, lines{lines}, offsetX{0}, offsetY{0}
    {}

    void setOffsets(const Player& player, const Map& map, const UI& ui);
    std::pair<int, int> getOffsets() const { return {offsetX, offsetY}; }
    std::pair<int, int> getDims() const { return {cols, lines}; }
};