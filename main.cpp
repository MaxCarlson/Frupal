#include <ncurses.h>
#include <iostream>
#include <thread>
#include <cstdint>
#include "map.h"
#include "input.h"
#include "display.h"
#include "player.h"
#include "camera.h"
#include "ui.h"
#include "items/itemloader.h"
#include "mapgenerator.h"

constexpr int FPS = 25;
constexpr int SleepTime = 1000 / FPS;

int main()
{

    initscr();
    keypad(stdscr, true);
    cbreak();
    noecho();
    curs_set(0);

    // TODO: Start screen

    // Main Loop
    //
    // Check for input
    // Apply user input
    // Check for player death
    // Print map/character
    // Print UI
    // Refresh
    // Run any other systems

    bool gameRunning = true;
    ItemLoader itemLoader;
    itemLoader.loadItems();
    
    while(gameRunning)
    {
        timeout(0);
        UI ui{COLS};
        Display display;
        uint32_t seed = 1;

        ui.mainMenu(display, gameRunning, seed);
        if(!gameRunning)
            break;

        MapGenerator mgen{128, seed, itemLoader};
        Map map = mgen.generate(400, 100);
        Input   input;
        Player  player{mgen.getPlayerCoords()};
        Camera  camera{COLS, LINES};

        bool first = true;
        for(;;)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds{SleepTime});

            if(!first && !input.input(player, map))
            {
                gameRunning = false;
                break;
            }

            // Toggle playerDeath on and off in player.h
            if(player.showPlayerDeath() && player.getEnergy() <= 0)
            {
                // Player has chosen to start a new game
                if(display.deathScreen() == 1)
                    break; 
                // Player has chosen to quit the program
                else
                {
                    gameRunning = false;
                    break;
                }
            }

            // This must come first so everything is printed properly
            camera.setOffsets(player, map, ui);

            player.discoverTerrrain(map);
            display.printMap(camera, map, ui);
            display.printCharacter(camera, player);
            display.printUI(camera, ui, player, map);
            first = false;
        }
    }

    clear();
    endwin();
    return 0;
}
