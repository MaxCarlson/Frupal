#pragma once
#include "map.h"

class mapStoreAndLoad 
{
  public:
  //Variables
  Map map;

  //Constructor
  mapStoreAndLoad() = default;

  //Functions
  int load(Map& map);
  int save(Map& map);

  private:
};
