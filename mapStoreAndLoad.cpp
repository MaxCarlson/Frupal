#include "mapStoreAndLoad.h"
#include "items/itemloader.h"
#include "items/chest.h"
#include <fstream>
#include <iostream>
#include <sstream>
//#include <tuple>

bool mapStoreAndLoad::save(Map& map, Player& player, const std::string fileName) {
  //using StringTuple = std::tuple<std::string,std::string,std::string,std::string>;
  //File variables
  std::ofstream outFile;
  char Delimiter = '|';
  outFile.exceptions ( std::ofstream::failbit | std::ofstream::badbit );
  try {
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
      outFile << map.getWidth() << " ";
      outFile << map.getHeight() << "\n";
      MapSquare squareToSave;
      //Nested loop to iterate through every map square.
      for(int i = 0; i < map.getHeight(); ++i) {
        for(int j = 0; j < map.getWidth(); ++j) {
          //Save square info.
          outFile << "SQUAREDATA" << Delimiter;
          squareToSave = map.sq(j, i);
          //outFile << j << " " << i << " ";
          outFile << squareToSave.discovered << " ";
          outFile << static_cast<int>(squareToSave.terrain);
          outFile << Delimiter;
          //If there is an item on the square.
          if(squareToSave.item != nullptr) {
            outFile << "ITEMDATA" << Delimiter;
            outFile << squareToSave.item->getName();
            outFile << Delimiter;
            //if(squareToSave.item->getName().compare("Chest") == true) {
            //If the item is a chest, save description.
            if(dynamic_cast<Chest*>(squareToSave.item)) {
              outFile << "CHESTDATA" << Delimiter;
              Chest* chestToSave = dynamic_cast<Chest*>(squareToSave.item);
              outFile << chestToSave->getValue();
              outFile << Delimiter;
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
          ss.str("");
          ss.clear();
        }
        //If we are at this line.
        if(!parsedLine.compare("TOOLDATA")) {
          do {
            //Set up stringstream.
            getline(inFile, parsedLine, Delimiter);
            ss.str(parsedLine);
            //std::cout << parsedLine << "\n";
            //Save tools.
            ss >> toolName >> toolType >> toolCost >> toolRating;
            //Load tools.
            tools.emplace_back(new Tool(toolName, toolType, toolCost, toolRating));
          }
          //Continue until we hit the end of the line.
          while(inFile.peek() != '\n');
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
        ss.str("");
        ss.clear();
        //Save map width and height.
        getline(inFile, parsedLine);
        ss.str(parsedLine);
        //std::cout << parsedLine << "\n";
        //std::cout << parsedLine << "\n";
        ss >> mapWidth;
        ss >> mapHeight;
        //std::cout << mapWidth;
        //getline(inFile, wholeLine); 
        //std::cout << wholeLine << "\n";
        //getline(inFile, parsedLine, Delimiter); 
        /*
        getline(inFile, parsedLine, Delimiter); 
        std::cout << parsedLine << "\n";
        */
        //Construct new map.
        //map.resetMap(mapWidth, mapHeight);
        //Save map tiles.
        for(int i = 0; i < mapHeight; ++i) {
          for(int j = 0; j < mapWidth; ++j) {
            do {
              getline(inFile, parsedLine, Delimiter); 
              //std::cout << parsedLine << "\n";
              //std::cout << "Beginning :" << parsedLine << "\n";
              if(!parsedLine.compare("SQUAREDATA")) {
                //Get the square data.
                std::cout << parsedLine << " ";
                getline(inFile, parsedLine, Delimiter); 
                ss.str(parsedLine);
                std::cout << parsedLine << " ";
                //Load square data into current map tile.
                ss >> mapDiscovered;
                ss >> mapTerrainInt;
                std::cout << map.sq(j, i).discovered << " ";
              }
              else if(parsedLine.compare("ITEMDATA") == 0) {
                //Get the item data.
                std::cout << parsedLine << " ";
                std::cout << parsedLine << " ";
                getline(inFile, parsedLine, Delimiter); 
                ss.str(parsedLine);
                std::cout << parsedLine << " ";
              }
              else if(parsedLine.compare("CHESTDATA") == 0) {
                //Get the chest data.
                std::cout << parsedLine << " ";
                getline(inFile, parsedLine, Delimiter); 
                ss.str(parsedLine);
                std::cout << parsedLine << " ";
              }
              map.sq(j, i).discovered = mapDiscovered;
              ss.clear();
            }
            while(inFile.peek() != '\n');
            std::cout << "\n";
            getline(inFile, parsedLine, '\n'); 
            break;
          }
          //Move to next line.
          //std::cout << "End: " << parsedLine << "\n";
          ss.str("");
          ss.clear();
        }
      }
    }
    catch(std::ifstream::failure &e) {
      std::cerr << "Exception opening/reading/closing file\n";
      return false;
    }
    return true;
  }
