#include <ncurses.h>
#include <iostream>
#include <thread>
#include "map.h"
#include "input.h"
#include "display.h"
#include "player.h"
#include "camera.h"
#include "ui.h"
#include "mapgenerator.h"
#include "mapStoreAndLoad.h"

constexpr int FPS = 25;
constexpr int SleepTime = 1000 / FPS;

int main()
{
  MapGenerator mgen{128, 13};
  Map map = mgen.generate(400, 100);
  Player  player{mgen.getPlayerCoords()};
  
  mapStoreAndLoad saveLoad;
  saveLoad.save(map, player, "mapSave.txt");

  
    return 0;
}
