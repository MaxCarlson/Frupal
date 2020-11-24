#pragma once
#include "map.h"
#include <string>
#include <fstream>

class mapStoreAndLoad 
{
  public:
  //Variables

  //Constructor
  //mapStoreAndLoad(Map& map);
  mapStoreAndLoad() = default;

  //Functions
  int load(Map& map, const std::string fileName);
  int save(Map& map, const std::string fileName);

  private:
};
