#include "input.h"
#include "player.h"
#include "map.h"
#include "movement.h"
#include <ncurses.h>
#include <iostream>

// Returns false if exit key is pressed. Handles all input
bool Input::input(Player& player, Map& map)
{
    int ch = getch();
    //std::cout << ch;

    // Remove excess input, make character easier to control
    flushinp();

    switch(ch)
    {
        case 'q':
            return false;

        case KEY_LEFT:
            if(player.getX() > 0)
                Movement::movePlayer(player, map, -1, 0);
            break;

        case KEY_RIGHT:
            if(player.getX() < map.getWidth() - 1)
                Movement::movePlayer(player, map, 1, 0);
            break;

        case KEY_UP:
            if(player.getY() > 0)
                Movement::movePlayer(player, map, 0, -1);
            break;

        case KEY_DOWN:
            if(player.getY() < map.getHeight() - 1)
                Movement::movePlayer(player, map, 0, 1);
            break;

        case 't':
            player.toggleTool();
            break;

        // TODO: Apparently these are supposed to be arrow keys, and number keys are used for movement?
        // TODO: Also, these should move the cursor in that dir, not set the cursor next to the player in that dir
        case 49: // 1
            player.setDir(Direction::NORTH);
            break;

        case 50: // 2
            player.setDir(Direction::EAST);
            break;

        case 51: // 3
            player.setDir(Direction::SOUTH);
            break;

        case 52: // 4
            player.setDir(Direction::WEST);
            break;

        case -1: // Default ERR input, just here for debugging so we can catch unknown key cods in defualt
            break;
        default:
            break;
    }

    return true;
}

bool Input::buyItem()
{
    int ch = 0;

    while(ch != 'y' && ch != 'n')
        ch = getch();

    if(ch == 'y')
        return true;
    return false;
}

bool Input::canBreakObstacle(Player& player, Obstacle *obstacle, int obstacleCost)
{
    if(player.hasTools())
    {
        // Allow player to use tools
        // Each time a tool is used, change obstacleCost according to tool modifier.
        // Delete tool after usage.
    }
     
    int ch = 0;
    int rating;
    int toolHelp = 0;
    // Show player options in UI:
    // "T": Toggle Tool
    // "U": Use Tool
    // "D": Done/Break Obstacle
    while(ch != 'd' || obstacleCost <= toolHelp)
    {
        // print obstacleCost to UI, so player knows
        // how much energy they need to spend
        ch = getch();
        switch(ch)
        {
            case 't':
                // Need to show tool names as they are being toggled through
                // just like in ui.cpp.
                player.toggleTool();
                break;

            case 'u':
                rating = player.useTool(obstacle);
                // tool is not compatible. Inform Player in UI
                if(rating < 0)
                {
                    rating = 0;
                    break;
                }
                toolHelp = toolHelp + (rating * 10);
                break;
        }
        obstacleCost = obstacleCost - toolHelp;
    }

    if(obstacleCost <= toolHelp)
        return true;

    if(player.getEnergy() >= obstacleCost)
    {
        player.modifyEnergy(-obstacleCost);
        return true;
    }
    return false;

    

}
    

    
