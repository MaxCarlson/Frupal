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

    // TODO: Start screen
    // Load map
    // Generate clues

    MapGenerator mgen{128, 2};
    Map m = mgen.voronoi(128, 200, 75);
    

    // Main Loop
    //
    // Check for input
    // Apply user input
    // Print map/character
    // Print UI
    // Refresh
    // Run any other systems

    UI      ui{COLS};
    //Map     map{128, 128};
    Map map{std::move(m)};
    Input   input;
    Player  player;
    Camera  camera{COLS, LINES};
    Display display;

    for(;;)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds{SleepTime});

        if(!input.input(player, map))
            break;

        // This must come first so everything is printed properly
        camera.setOffsets(player, map, ui);

        player.discoverTerrrain(map);
        display.printMap(camera, map, ui);
        display.printCharacter(camera, player);
        display.printUI(camera, ui, player, map);
    }

    endwin();
    return 0;
}