#include "input.h"
#include "player.h"
#include "map.h"
#include "movement.h"
#include "ui.h"
#include "camera.h"
#include <ncurses.h>
#include <iostream>

// Returns false if exit key is pressed. Handles all input
bool Input::input(Player& player, Map& map, UI& ui, Camera& camera)
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
                Movement::movePlayer(player, map, ui, camera, -1, 0);
            break;

        case KEY_RIGHT:
            if(player.getX() < map.getWidth() - 1)
                Movement::movePlayer(player, map, ui, camera, 1, 0);
            break;

        case KEY_UP:
            if(player.getY() > 0)
                Movement::movePlayer(player, map, ui, camera, 0, -1);
            break;

        case KEY_DOWN:
            if(player.getY() < map.getHeight() - 1)
                Movement::movePlayer(player, map, ui, camera, 0, 1);
            break;

        case 't':
            // Change active tool
            break;

        // TODO: Apparently these are supposed to be arrow keys, and number keys are used for movement?
        // TODO: Also, these should move the cursor in that dir, not set the cursor next to the player in that dir
        case 49: // 1
            player.setCursor(player.getX(), player.getY() + 1);  //move cursor to Direction::NORTH
            break;

        case 50: // 2
            player.setCursor(player.getX(), player.getY() - 1); //Direction::EAST);
            break;

        case 51: // 3
            player.setCursor(player.getX(), player.getY() - 1); //Direction::SOUTH);
            break;

        case 52: // 4
            player.setCursor(player.getX(), player.getY() + 1); //Direction::WEST);
            break;

        case -1: // Default ERR input, just here for debugging so we can catch unknown key cods in defualt
            break;
        default:
            break;
    }

    return true;
}

bool Input::buyItem(const Camera & camera, const UI& ui)
{
    int ch = 0;
    auto [cx, cy] = camera.getDims();

    int xOffset = cx - ui.getSize() + 2;  
    std::string prompt = "Press Y or N"; 
    std::string prompt1 = "To buy item."; 
    mvaddstr(10, xOffset, prompt.c_str()); 
    mvaddstr(11, xOffset, prompt1.c_str()); 
    
    while(ch != 'y' && ch != 'n')
        ch = getch();
    if(ch != 'y')
        return false; 
    return true;  
}

bool Input::canBreakObstacle(Player& player, int obstacleCost)
{
    if(player.getEnergy() >= obstacleCost)
    {
        player.modifyEnergy(-obstacleCost);
        return true;
    }
    return false;
    // Check if player has a tool compatible with obstacle
    // If no - If player cannot afford to remove obstacle, return false
    // otherwise, return true.
    //
    // If yes, ask if player would like to use the tool
    // If no, If player can destroy obstacle using current energy, return true. Else, return false
    // If yes, check if player can destroy obstacle using tool
    // benefits combined with current energy. If yes, return true. Else, return false.
}
