#pragma once
#include "items/tool.h"
#include <vector>

class Map;

enum Direction
{
    NORTH,
    EAST,
    SOUTH,
    WEST
};

class Player
{
    int energy;
    int money;
    int x, y;
    bool hasBinoculars;
    bool onShip;
    bool playerDeath;
    int px, py;
    std::vector<Tool*> tools;
    
public:

    Player(std::pair<int, int> xy) :
        energy{100}, money{1000}, x{xy.first}, y{xy.second}, hasBinoculars{false}, onShip{false}, playerDeath{true}, px{xy.first}, py{xy.second},  tools{}
    {} 

    int getX() const { return x; }
    int getY() const { return y; }
    void setX(int v) { x = v; }
    void setY(int v) { y = v; }
    
    void modifyMoney(int v)     { money     += v; }
    void modifyEnergy(int v)    { energy    += v; }

    int getMoney()  const { return money; }
    int getEnergy() const { return energy; }
    std::pair<int, int> getCursor() const { return std::pair{px, py}; }
    void setCursor(int tpx, int tpy) {px = tpx; py = tpy;}
    bool showPlayerDeath() const { return playerDeath; }

    std::pair<int, int> selectedSquare() const;
    std::pair<int, int> getXY() const { return {x, y}; }
    void discoverTerrrain(Map& map);

    void addTool(Tool *&tool);
    void boughtBinoculars() { hasBinoculars = true; }
    
};
