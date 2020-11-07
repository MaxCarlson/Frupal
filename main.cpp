#include <ncurses.h>
#include <iostream>
#include <thread>
//#include "map.h"
//#include "camera.h"
#include "input.h"

constexpr int FPS = 20;
constexpr int SleepTime = 1000 / FPS;

int main()
{

    initscr();
    refresh();
    keypad(stdscr, true);
    cbreak();
    noecho();

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

    Input input;

    for(;;)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds{SleepTime});

        if(!input.input())
            break;

    }

    endwin();
 
    return 0;
}