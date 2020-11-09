#include "input.h"
#include <ncurses.h>
#include "player.h"
#include "map.h"
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
                player.setX(player.getX() - 1);
            break;

        case KEY_RIGHT:
            if(player.getX() < map.getWidth() - 1)
                player.setX(player.getX() + 1);
            break;

        case KEY_UP:
            if(player.getY() > 0)
                player.setY(player.getY() - 1);
            break;

        case KEY_DOWN:
            if(player.getY() < map.getHeight() - 1)
                player.setY(player.getY() + 1);
            break;

        case 't':
            // Change active tool
            break;

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