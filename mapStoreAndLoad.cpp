#include "mapStoreAndLoad.h"

//Constructor
/*
mapStoreAndLoad::mapStoreAndLoad(Map& map) {
  this->map = map;
}
*/


bool mapStoreAndLoad::save(Map& map, const std::string fileName) {
  std::ofstream outFile;
  outFile.open(fileName);

  return 1;
}

bool mapStoreAndLoad::load(Map& map, const std::string fileName) {
  return 1;
}
