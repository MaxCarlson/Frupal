#pragma once
#include <vector>
#include <string>

class Map;
class Tool;
class Obstacle;
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
    int toolIDX;
    bool hasBinoculars;
    bool onShip;
    bool playerDeath;
    int px, py;
    std::vector<Tool*> tools;

public:

    Player() = default;

    Player(std::pair<int, int> xy) :
        energy{100}, money{1000}, x{xy.first}, y{xy.second}, toolIDX{0}, hasBinoculars{false}, onShip{false}, playerDeath{true}, px{xy.first}, py{xy.second}, tools{}
    {} 

//Constructor for map loading.
    Player(int energy, int money, int x, int y, bool hasBinoculars, bool onShip, bool playerDeath, int px, int py, std::vector<Tool*> tools);

    int getX() const { return x; }
    int getY() const { return y; }
    int getToolIDX() const { return toolIDX; }
    void setX(int v) { x = v; }
    void setY(int v) { y = v; }
    int getPX() const { return px; }
    int getPY() const { return py; }
    
    void modifyMoney(int v)     { money     += v; }
    void modifyEnergy(int v)    { energy    += v; }

    int getMoney()  const { return money; }
    int getEnergy() const { return energy; }
    std::pair<int, int> getCursor() const { return std::pair{px, py}; }
    void setCursor(const Map& map, int tpx, int tpy); 
    bool showPlayerDeath() const { return playerDeath; }
    bool hasTools();
    bool toolTypeMatch(Obstacle * obstacle);

    std::pair<int, int> selectedSquare() const;
    std::pair<int, int> getXY() const { return {x, y}; }
    void discoverTerrrain(Map& map);
    void addTool(Tool *tool);

    void boughtBinoculars() { hasBinoculars = true; }
    bool getHasBinoculars() const { return hasBinoculars; }
    bool getOnShip() const { return onShip; }
    std::vector<Tool*> getTools() const { return tools; }
    int toggleTool();
    int useTool(Obstacle * obstacle);
    std::string playerToolName() const; 
};

