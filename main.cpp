#include <ncurses.h>
#include <iostream>
#include <thread>
#include "map.h"
#include "input.h"
#include "display.h"
#include "player.h"
#include "camera.h"
#include "ui.h"
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
    while(gameRunning)
    {
        timeout(0);
        
        UI      ui{COLS};
        MapGenerator mgen{128, 13};
        Map map = mgen.generate(400, 100);
        Input   input;
        Player  player{mgen.getPlayerCoords()};
        Camera  camera{COLS, LINES};
        Display display;

        for(;;)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds{SleepTime});

            if(!input.input(player, map))
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
        }
    }

    endwin();
    return 0;
}
