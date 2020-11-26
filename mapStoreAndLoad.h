#pragma once
#include "map.h"
#include "player.h"
#include <string>

class mapStoreAndLoad 
{
  public:
  //Variables

  //Constructor
  //mapStoreAndLoad(Map& map);
  mapStoreAndLoad() = default;

  //Functions
  /**
    * Saves to a given file from a given map.
    * @param map Address of the map to save.
    * @param fileName Name of the file to load from.
    * @return Returns true if the map saves succesfully, false otherwise.
    */
  bool save(Map& map, Player& player, const std::string fileName);
  /**
    * Loads a map from a given file.
    * @param map Address of the map to load.
    * @param fileName Name of the file to load from.
    * @return Returns true if the map loads succesfully, false otherwise.
    */
  bool load(Map& map, const std::string fileName);

  private:
};
