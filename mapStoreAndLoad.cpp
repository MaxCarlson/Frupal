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
