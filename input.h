#pragma once

class Player;
class Map;

class Input
{
    enum Mode
    {
        NORMAL,
        UI
    };

    Mode mode;

public:
    Input() :
        mode{NORMAL}
    {}

    bool input(Player& player, Map& map);
};