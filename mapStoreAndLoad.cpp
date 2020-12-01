#include "mapStoreAndLoad.h"
#include "items/chest.h"
#include <fstream>
#include <iostream>

bool mapStoreAndLoad::save(Map& map, Player& player, const std::string fileName) {
  std::ofstream outFile;
  outFile.exceptions ( std::ofstream::failbit | std::ofstream::badbit );
  try {
    outFile.open(fileName);

    if(outFile.is_open()) {
      //Player info.
      outFile << player.getEnergy() << " ";
      outFile << player.getMoney() << " ";
      outFile << player.getX() << " ";
      outFile << player.getY() << " ";
      outFile << player.getHasBinoculars() << " ";
      outFile << player.getOnShip() << " ";
      outFile << player.showPlayerDeath() << " ";
      outFile << static_cast<int>(player.getDir()) << " ";
      //Not sure if this works properly, should save all tools.
      for(const auto &e : player.getTools()) outFile << e->getName() << " ";
      outFile  << "\n";

      //Map info.
      outFile << map.getWidth() << " ";
      outFile << map.getHeight() << "\n";
      MapSquare squareToSave;
      for(int i = 0; i < map.getHeight(); ++i)
      {
        for(int j = 0; j < map.getWidth(); ++j) {
          squareToSave = map.sq(j, i);
          outFile << j << " " << i << " ";
          outFile << squareToSave.discovered << " ";
          outFile << static_cast<int>(squareToSave.terrain) << " ";
          if(squareToSave.item != nullptr) {
            if(squareToSave.item->getName().compare("Chest") == true) {
              outFile << squareToSave.item->getName() << " ";

            }
            else {
            //outFile << squareToSave.item->getCh() << " ";
            outFile << squareToSave.item->getName() << " ";
           }
          }
          outFile  << "\n";
        }
      }
    }
    else {
      std::cerr << "ERROR:" << fileName << " did not open.";
    }

    outFile.close();
  }
  catch(std::ofstream::failure &e) {
    std::cerr << "Exception opening/reading/closing file\n";
    return false;
  }

  return true;
}

bool mapStoreAndLoad::load(Map& map, const std::string fileName) {
  return true;
}
