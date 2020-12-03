#include "mapStoreAndLoad.h"
#include "items/chest.h"
#include <fstream>
#include <iostream>
#include <tuple>

bool mapStoreAndLoad::save(Map& map, Player& player, const std::string fileName) {
  //using StringTuple = std::tuple<std::string,std::string,std::string,std::string>;
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
      //Nested loop to iterate through every map square.
      for(int i = 0; i < map.getHeight(); ++i)
      {
        for(int j = 0; j < map.getWidth(); ++j) {
          //Save square info.
          squareToSave = map.sq(j, i);
          outFile << j << " " << i << " ";
          outFile << squareToSave.discovered << " ";
          outFile << static_cast<int>(squareToSave.terrain) << " ";
          //If there is an item on the square.
          if(squareToSave.item != nullptr) {
            outFile << squareToSave.item->getName() << " ";
            //if(squareToSave.item->getName().compare("Chest") == true) {
            //If the item is a chest, save description.
            if(dynamic_cast<Chest*>(squareToSave.item)) {
              Chest* chestToSave = dynamic_cast<Chest*>(squareToSave.item);
              outFile << chestToSave->getValue() << " ";
              //StringTuple descriptionToSave = squareToSave.item->getDescription();
            }
            //outFile << squareToSave.item->getCh() << " ";
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
