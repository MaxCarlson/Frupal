#include "mapStoreAndLoad.h"
#include <fstream>
#include <iostream>

//Constructor
/*
mapStoreAndLoad::mapStoreAndLoad(Map& map) {
  this->map = map;
}
*/


bool mapStoreAndLoad::save(Map& map, const std::string fileName) {
  std::ofstream outFile;
  outFile.exceptions ( std::ofstream::failbit | std::ofstream::badbit );
  try {
  outFile.open(fileName);

  if(outFile.is_open()) {
    outFile << map.getWidth() << " ";
    outFile << map.getHeight() << "\n";

    for(int i = 0; i < map.getHeight(); ++i)
    {
        for(int j = 0; j < map.getWidth(); ++j) {
          MapSquare squareToSave = map.sq(i, j);
          //Save coordinates.
          outFile << j << " " << i << " ";
          outFile << squareToSave.discovered << " ";
          //Save terrain type as an integer.
          outFile << static_cast<int>(squareToSave.terrain) << " ";
          //If there is an item on the square.
          if(squareToSave.item != nullptr) {
          outFile << squareToSave.item->getCh() << " ";
          outFile << squareToSave.item->getName() << " ";
          }
          outFile << "\n";
            //delete map[i][j].item;
        }
        //delete[] map[i];
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
