#include "mapStoreAndLoad.h"
#include "items/itemloader.h"
#include "items/chest.h"
#include "items/tool.h"
#include "items/food.h"
#include "items/obstacle.h"
#include "items/binoculars.h"
#include "items/ship.h"
#include "items/diamond.h"
#include "items/clue.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <sys/stat.h>

// Function: fileExists
/**
  Check if a file exists
  @param[in] filename - the name of the file to check

  @return    true if the file exists, else false

 */

int is_empty(const char *s) {
  while (*s != '\0') {
    if (!isspace((unsigned char)*s))
      return 0;
    s++;
  }
  return 1;
}

bool fileExists(const std::string& filename)
{
  struct stat buf;
  if (stat(filename.c_str(), &buf) != -1)
  {
    return true;
  }
  return false;
}

/*
   bool fileExists(std::string fileName)
   {
   std::ifstream infile(fileName);
//std::cout << fileName << "\n";
//std::cout << infile.good() << "\n";
return infile.good();
}
 */

/*
   bool appendFileName(std::string filename, std::string filenameToReplace) {
   std::string temp;
//Find the '_' character, if there is not one, filename invalid.
if(filename.find('_') == false) {
return false;     
}
//Get the position of the number to replace.
int toReplace = filename.find('_') + 1;
//Increment number ascii.
char numberIncrement = filename.at(toReplace) + 1;
//Replace doesn't like characters so add to temp string.
temp.push_back(numberIncrement);
//Replace number.
filenameToReplace = filename.replace(toReplace, 1, temp);
//std::cout << filename << "\n";
//numberIncrement++ ;
//filename += '_';
//filename += numberIncrement;
std::cout << filename << "\n";
return true;
}
 */

bool mapStoreAndLoad::save(Map& map, Player& player, std::string fileName) {
  //using StringTuple = std::tuple<std::string,std::string,std::string,std::string>;
  //bool whileEnd = false;
  //std::string modifiedFileName = fileName;
  //std::string appendedFileName;
  //File variables
  std::ofstream outFile;
  char Delimiter = '|';
  outFile.exceptions ( std::ofstream::failbit | std::ofstream::badbit );
  try {

    /*
       while(whileEnd == false) {
       appendFileName(modifiedFileName, appendedFileName);
       std::cout << appendedFileName << "\n";
       if(fileExists(appendedFileName) == false) {
       whileEnd = true; 
       }
       }
     */

    /*
       if(fileExists(fileName)) {
       char numberIncrement = fileName.back();
       numberIncrement++ ;
       fileName += '_';
       fileName += numberIncrement;
       }
     */

    outFile.open(fileName);
    if(outFile.is_open()) {
      //Will use a title and delimiter | to specify blocks of variables.
      //Player info.
      outFile << "PLAYERDATA" << Delimiter;
      outFile << player.getEnergy() << " ";
      outFile << player.getMoney() << " ";
      outFile << player.getX() << " ";
      outFile << player.getY() << " ";
      outFile << player.getHasBinoculars() << " ";
      outFile << player.getOnShip() << " ";
      outFile << player.showPlayerDeath() << " ";
      outFile << static_cast<int>(player.getDir());
      outFile << Delimiter;
      //Not sure if this works properly, should save all tools.
      outFile << "TOOLDATA" << Delimiter;
      for(const auto &e : player.getTools()) outFile << e->getName() << " " << e->getType() << " " << e->getCost() << " " << e->getRating() << Delimiter;
      outFile  << "\n";

      //Map info.
      outFile << map.getHeight() << " ";
      outFile << map.getWidth() << "\n";
      MapSquare squareToSave;
      //Nested loop to iterate through every map square.
      for(int i = 0; i < map.getHeight(); ++i) {
        for(int j = 0; j < map.getWidth(); ++j) {
          //Save square info.
          outFile << "SQUAREDATA" << Delimiter;
          squareToSave = map.sq(i, j);
          outFile << i << " " << j << " ";
          outFile << squareToSave.discovered << " ";
          outFile << static_cast<int>(squareToSave.terrain);
          outFile << Delimiter;
          //If there is an item on the square.
          if(squareToSave.item != nullptr) {
            mapItem = squareToSave.item;
            //if(squareToSave.item->getName().compare("Chest") == true) {
            //If the item is a chest, save description.
            if(dynamic_cast<Tool*>(mapItem)) {
              Tool* toolToSave = dynamic_cast<Tool*>(mapItem);
              //map.sq(i, j).item = new Tool(toolToSave->getName(), toolToSave->getType(), toolToSave->getCost(), toolToSave->getRating());
              outFile << "ITEMTYPE" << Delimiter;
              outFile << "TOOL";
              outFile << Delimiter;
              outFile << "ITEMNAME";
              outFile << Delimiter;
              outFile << toolToSave->getName();
              outFile << Delimiter;
              outFile << "ITEMDATA" << Delimiter;
              outFile << toolToSave->getType() << " ";
              outFile << toolToSave->getCost() << " ";
              outFile << toolToSave->getRating();
            }
            else if(dynamic_cast<Food*>(mapItem)) {
              Food* foodToSave = dynamic_cast<Food*>(mapItem);
              outFile << "ITEMTYPE" << Delimiter;
              outFile << "FOOD";
              outFile << Delimiter;
              outFile << "ITEMNAME";
              outFile << Delimiter;
              outFile << foodToSave->getName();
              outFile << Delimiter;
              outFile << "ITEMDATA" << Delimiter;
              //map.sq(i, j).item = new Food(foodToSave->getName(), foodToSave->getCost(), foodToSave->getEnergy());
              outFile << foodToSave->getCost() << " ";
              outFile << foodToSave->getEnergy();
            }
            else if(dynamic_cast<Obstacle*>(mapItem)) {
              Obstacle* obstacleToSave = dynamic_cast<Obstacle*>(mapItem);
              outFile << "ITEMTYPE" << Delimiter;
              outFile << "OBSTACLE";
              outFile << Delimiter;
              outFile << "ITEMNAME";
              outFile << Delimiter;
              outFile << obstacleToSave->getName();
              outFile << Delimiter;
              outFile << "ITEMDATA" << Delimiter;
              //map.sq(i, j).item = new Obstacle(obstacleToSave->getName(), obstacleToSave->getType(), obstacleToSave->getEnergy());
              outFile << obstacleToSave->getType() << " ";
              outFile << obstacleToSave->getEnergy();
            }
            else if(dynamic_cast<Binoculars*>(mapItem)) {
              Binoculars* binocularsToSave = dynamic_cast<Binoculars*>(mapItem);
              outFile << "ITEMTYPE" << Delimiter;
              outFile << "BINOCULARS";
              outFile << Delimiter;
              outFile << "ITEMDATA" << Delimiter;
              //map.sq(i, j).item = new Binoculars(binocularsToSave->getName(), binocularsToSave->getCost());
              outFile << binocularsToSave->getCost();
            }
            else if(dynamic_cast<Chest*>(mapItem)) {
              Chest* chestToSave = dynamic_cast<Chest*>(mapItem);
              outFile << "ITEMTYPE" << Delimiter;
              outFile << "CHEST";
              outFile << Delimiter;
              outFile << "ITEMDATA" << Delimiter;
              outFile << chestToSave->getValue();
            }
            else {
              outFile << "ITEMNAME" << Delimiter;
              outFile << squareToSave.item->getName();
            }
            outFile << Delimiter;
            //StringTuple descriptionToSave = squareToSave.item->getDescription();
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

  bool mapStoreAndLoad::load(Map& map, Player& player, const std::string fileName) {
    //File variables
    char Delimiter = '|';
    //std::string filePosition;
    //char filePosition[256];
    std::ifstream inFile;
    std::stringstream ss;
    //std::string wholeLine;
    std::string parsedLine;

    //Player variables.
    int energy = 0;
    int money = 0;
    int x = 0;
    int y = 0;
    bool hasBinoculars = false;
    bool onShip = false;
    bool playerDeath = false;
    Direction dir = NORTH;
    int dirInt = 0;

    //Tool variables.
    std::vector<Tool*> tools;
    std::string toolName;
    std::string toolType;
    int toolCost = 0;
    int toolRating = 0;

    //Map variables
    int mapWidth = 0;
    int mapHeight = 0;
    int mapDiscovered = 0;
    int mapTerrainInt = 0;
    int mapX = 0;
    int mapY = 0;

    //Item variables
    std::string itemName;
    std::string itemType;
    int itemCost = 0;
    int itemRating = 0;
    int itemEnergy = 0;
    int itemValue = 0;
    std::string itemClassType;

    inFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    try {
      inFile.open(fileName);
      if(inFile.is_open()) {
        getline(inFile, parsedLine, Delimiter);
        ss.str(parsedLine);
        //std::cout << parsedLine << "\n";
        //If we are at this line.
        if(!parsedLine.compare("PLAYERDATA")) {
          //Set up stringstream.
          getline(inFile, parsedLine, Delimiter);
          ss.str(parsedLine);
          //Save Player info.
          ss >> energy;
          ss >> money;
          ss >> x;
          ss >> y;
          ss >> hasBinoculars;
          ss >> onShip;
          ss >> playerDeath;
          ss >> dirInt;
          dir = static_cast<Direction>(dirInt);
          //std::cout << parsedLine << "\n";
          //inFile >> energy;
          //inFile >> money;
          //inFile >> x;
          //inFile >> y;
          //inFile >> hasBinoculars;
          //inFile >> onShip;
          //inFile >> playerDeath;
          //inFile >> dirInt;

          //Set up next line.
          //std::cout << parsedLine << "\n";
          getline(inFile, parsedLine, Delimiter);
          //std::cout << parsedLine << "\n";
        }
        //If we are at this line.
        if(!parsedLine.compare("TOOLDATA")) {
          while(inFile.peek() != '\n'){
            //Set up stringstream.
            ss.clear();
            getline(inFile, parsedLine, Delimiter);
            if(!is_empty(parsedLine.c_str())) {
              ss.str(parsedLine);
              //std::cout << parsedLine << "\n";
              //Save tools.
              ss >> toolName >> toolType >> toolCost >> toolRating;
              //Load tools.
              tools.emplace_back(new Tool(toolName, toolType, toolCost, toolRating));
            }
          }
          //Continue until we hit the end of the line.

          //Go to next line.
          getline(inFile, parsedLine);
          //inFile >> toolName >> toolType >> toolCost >> toolRating;
          //Save tools.
          //Load player info.
          //std::cout << tools[0]->getName() << "\n";
        }
        //Initialize player with data.
        player = Player(energy, money, x, y, hasBinoculars, onShip, playerDeath, dir, tools);
        //Reset stringstream.
        ss.clear();
        //Save map width and height.
        getline(inFile, parsedLine);
        ss.str(parsedLine);
        //std::cout << parsedLine << "\n";
        //std::cout << parsedLine << "\n";
        ss >> mapHeight;
        ss >> mapWidth;
        //std::cout << mapWidth;
        //getline(inFile, wholeLine); 
        //std::cout << wholeLine << "\n";
        //getline(inFile, parsedLine, Delimiter); 
        /*
           getline(inFile, parsedLine, Delimiter); 
           std::cout << parsedLine << "\n";
         */
        //Reset stringstream.
        ss.clear();
        //Construct new map.
        map.resetMap(mapWidth, mapHeight);
        //Save map tiles.
        for(int i = 0; i < mapHeight; ++i) {
          for(int j = 0; j < mapWidth; ++j) {
            while(inFile.peek() != '\n') {
              getline(inFile, parsedLine, Delimiter); 
              //std::cout << parsedLine << "\n";
              //std::cout << "Beginning :" << parsedLine << "\n";
              if(parsedLine.compare("SQUAREDATA") == 0) {
                //Get the square data.
                //                std::cout << parsedLine << " ";
                getline(inFile, parsedLine, Delimiter); 
                ss.clear();
                if(!is_empty(parsedLine.c_str())) {
                  ss.str(parsedLine);
                  //                std::cout << parsedLine << " ";
                  //Load square data into current map tile.
                  ss >> mapY;
                  ss >> mapX;
                  ss >> mapDiscovered;
                  ss >> mapTerrainInt;
                  //Load map variables in.
                  map.sq(i, j).discovered = mapDiscovered;
                  map.sq(i, j).terrain = static_cast<Terrain>(mapTerrainInt);
                  //std::cout << map.sq(i, j).discovered << " ";
                  //Terrain tempTerrain = map.sq(i, j).terrain;
                  //std::cout << static_cast<int>(tempTerrain) << " ";
                }
              }
              else if(parsedLine.compare("ITEMTYPE") == 0) {
                //Get the item type.
                //                std::cout << parsedLine << " ";
                getline(inFile, parsedLine, Delimiter); 
                ss.clear();
                if(!is_empty(parsedLine.c_str())) {
                  ss.str(parsedLine);
                  ss >> itemClassType;
                  //std::cout << parsedLine << " ";
                  //                std::cout << itemClassType << " ";
                  getline(inFile, parsedLine, Delimiter); 
                  //                std::cout << parsedLine << " ";
                  if(parsedLine.compare("ITEMNAME") == 0)  {
                    getline(inFile, parsedLine, Delimiter); 
                    itemName = parsedLine;
                    //                  std::cout << itemName << " ";
                    getline(inFile, parsedLine, Delimiter); 
                    //                  std::cout << parsedLine << " ";
                    if(itemClassType.compare("TOOL") == 0) {
                      //getline(inFile, parsedLine, Delimiter); 
                      getline(inFile, parsedLine, Delimiter); 
                      //                    std::cout << parsedLine << " ";
                      //getline(inFile, parsedLine, Delimiter); 
                      //Get the item type.
                      //std::cout << parsedLine << " ";
                      //getline(inFile, parsedLine, Delimiter); 
                      ss.clear();
                      ss.str(parsedLine);
                      ss >> itemType;
                      //std::cout << itemType << " ";
                      ss >> itemCost;
                      //std::cout << itemCost << " ";
                      ss >> itemRating;
                      //std::cout << itemRating << " ";
                      //Tool* toolToSave = dynamic_cast<Tool*>(mapItem);
                      map.sq(i, j).item = new Tool(itemName, itemType, itemCost, itemRating);
                      //map.sq(i, j).item = new Tool(toolToSave->getName(), toolToSave->getType(), toolToSave->getCost(), toolToSave->getRating());
                    }
                  }
                  else if(itemClassType.compare("FOOD") == 0) {
                    getline(inFile, parsedLine, Delimiter); 
                    //                    std::cout << parsedLine << " ";
                    ss.clear();
                    if(!is_empty(parsedLine.c_str())) {
                      ss.str(parsedLine);
                      ss >> itemCost;
                      //std::cout << itemCost << " ";
                      ss >> itemEnergy;
                      //Food* foodToSave = dynamic_cast<Food*>(mapItem);
                      map.sq(i, j).item = new Food(itemName, itemCost, itemEnergy);
                      //map.sq(i, j).item = new Food(foodToSave->getName(), foodToSave->getCost(), foodToSave->getEnergy());
                    }
                  }
                  else if(itemClassType.compare("OBSTACLE") == 0) {
                    getline(inFile, parsedLine, Delimiter); 
                    //                    std::cout << parsedLine << " ";
                    ss.clear();
                    if(!is_empty(parsedLine.c_str())) {
                      ss.str(parsedLine);
                      ss >> itemType;
                      ss >> itemEnergy;
                      //Obstacle* obstacleToSave = dynamic_cast<Obstacle*>(mapItem);
                      map.sq(i, j).item = new Obstacle(itemName, itemType, itemEnergy);
                      //map.sq(i, j).item = new Obstacle(obstacleToSave->getName(), obstacleToSave->getType(), obstacleToSave->getEnergy());
                    }
                  }
                }
                else if(itemClassType.compare("BINOCULARS") == 0) {
                  getline(inFile, parsedLine, Delimiter); 
                  //                  std::cout << parsedLine << " ";
                  ss.clear();
                  if(!is_empty(parsedLine.c_str())) {
                    ss.str(parsedLine);
                    ss >> itemCost;
                    //Binoculars* binocularsToSave = dynamic_cast<Binoculars*>(mapItem);
                    map.sq(i, j).item = new Binoculars(itemName, itemCost);
                    //map.sq(i, j).item = new Binoculars(binocularsToSave->getName(), binocularsToSave->getCost());
                  }

                }
                else if(itemClassType.compare("CHEST") == 0) {
                  getline(inFile, parsedLine, Delimiter); 
                  //                  std::cout << parsedLine << " ";
                  ss.clear();
                  if(!is_empty(parsedLine.c_str())) {
                    ss.str(parsedLine);
                    ss >> itemValue;
                    //Chest* chestToSave = dynamic_cast<Chest*>(mapItem);
                    //map.sq(i, j).item = new Chest(chestToSave->getName(), chestToSave->getValue());
                    map.sq(i, j).item = new Chest(itemName, itemValue);
                  }
                }
              }
              else if (parsedLine.compare("ITEMNAME") == 0) {
                //Get the item type.
                //                std::cout << parsedLine << " ";
                getline(inFile, parsedLine, Delimiter); 
                ss.clear();
                if(!is_empty(parsedLine.c_str())) {
                  ss.str(parsedLine);
                  ss >> itemName;
                  //                std::cout << itemName;

                  if(itemName.compare("Ship") == 0) {
                    map.sq(i, j).item = new Ship(itemName);
                  }
                  else if(itemName.compare("Diamond") == 0) {
                    map.sq(i, j).item = new Diamond(itemName);
                  }
                  else if(itemName.compare("Clue") == 0) {
                    map.sq(i, j).item = new Clue(itemName);
                  }
                }
              }
            }
            //            std::cout << "\n";
            getline(inFile, parsedLine, '\n'); 
            ss.str("");
            ss.clear();
            //break;
          }
          //Move to next line.
          //std::cout << "End: " << parsedLine << "\n";
        }
      }
    }
    catch(std::ifstream::failure &e) {
      std::cerr << "Exception opening/reading/closing file\n";
      return false;
    }
    return true;
  }
