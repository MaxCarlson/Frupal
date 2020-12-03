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
    Direction dir; // TODO: Remove this and make it an xy coordinate pair for cursor location
    std::vector<Tool*> tools;

    
public:

    Player(std::pair<int, int> xy) :
        energy{100}, money{1000}, x{xy.first}, y{xy.second}, hasBinoculars{false}, onShip{false}, playerDeath{true}, dir{SOUTH}, tools{}
    {} 

//Constructor for map loading.
    Player(int energy, int money, int x, int y, bool hasBinoculars, bool onShip, bool playerDeath, Direction dir, std::vector<Tool*> tools);

    int getX() const { return x; }
    int getY() const { return y; }
    void setX(int v) { x = v; }
    void setY(int v) { y = v; }
    
    void modifyMoney(int v)     { money     += v; }
    void modifyEnergy(int v)    { energy    += v; }

    int getMoney()  const { return money; }
    int getEnergy() const { return energy; }
    Direction getDir() const { return dir; }
    void setDir(Direction dr) { dir = dr; }

    bool showPlayerDeath() const { return playerDeath; }

    std::pair<int, int> selectedSquare() const;
    std::pair<int, int> getXY() const { return {x, y}; }
    void discoverTerrrain(Map& map);

    void addTool(Tool *&tool);
    void boughtBinoculars() { hasBinoculars = true; }
    bool getHasBinoculars() const { return hasBinoculars; }
    bool getOnShip() const { return onShip; }
    std::vector<Tool*> getTools() const { return tools; }
    
};
