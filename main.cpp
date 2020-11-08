#include <ncurses.h>
#include <iostream>
#include <thread>
#include "map.h"
#include "input.h"
#include "display.h"
#include "player.h"
#include "camera.h"

constexpr int FPS = 20;
constexpr int SleepTime = 1000 / FPS;

int main()
{

    initscr();
    timeout(0);
    keypad(stdscr, true);
    cbreak();
    noecho();
    curs_set(0);

    // Start screen
    // Load map
    // Generate hints
    

    // Main Loop
    //
    // Check for input
    // Print map
    // Print UI
    // Apply user input
    // Run any other systems
    // Refresh

    Map     map{mapWidth, mapHeight};
    Input   input;
    Player  player;
    Camera  camera{&map, COLS, LINES};
    Display display;

    for(;;)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds{SleepTime});

        if(!input.input(player, map))
            break;

        camera.setOffsets(player);
        display.printMap(camera, map);
        display.printCharacter(camera, player);

    }

    endwin();
 
    return 0;
}