#include <ncurses.h>
#include <iostream>
#include <thread>
#include <cstdint>
#include "map.h"
#include "input.h"
#include "display.h"
#include "player.h"
#include "camera.h"
#include "ui.h"
#include "items/itemloader.h"
#include "mapgenerator.h"
#include "mapStoreAndLoad.h"

constexpr int FPS = 25;
constexpr int SleepTime = 1000 / FPS;

int main()
{
  ItemLoader itemLoader;
  itemLoader.loadItems();

  MapGenerator mgen{128, 13, itemLoader};
  mapStoreAndLoad saveLoad;

/*
  //Save testing.
  std::minstd_rand0 generator (2);
  Map map = mgen.generate(400, 100);
  Player  player{mgen.getPlayerCoords()};
  Tool* tool = new Tool(itemLoader.getTool(generator));
  player.addTool(tool);
  player.addTool(tool);
  saveLoad.save(map, player, "mapSave.txt");
  */

  //Load testing.
  Map map(400, 100);
  Player player;
  saveLoad.load(map, player, "mapSave.txt");
  std::cout << player.getEnergy() << " ";
  std::cout << player.getMoney() << " ";
  std::cout << "\n ";
  for(const auto &e : player.tools) {
    std::cout << e->getName() << " ";
  }




  return 0;
}
