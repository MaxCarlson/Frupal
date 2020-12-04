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
     
    int ch = 0;
    int rating;
    bool toolUsed = false;
    std::string match = "Use Tool? (U)";
    std::string notMatch = "Not Compatible!";
    std::string fist = "Break with fist? (F)";

    while(!toolUsed)
    {
        // ui stuff is a bit hacky but worked for all
        // screen sized that I tried
        move(LINES - 6,  COLS - 21);
        clrtoeol(); 
        mvaddstr(LINES - 6, COLS - 21, fist.c_str());
        if(player.toolTypeMatch(obstacle))
        {
            move(LINES - 5,  COLS - 21);
            clrtoeol(); 
            mvaddstr(LINES - 5, COLS - 21, match.c_str());

            move(LINES - 4,  COLS - 21);
            clrtoeol(); 
            mvaddstr(LINES - 4, COLS - 21, player.playerToolName().c_str());

        }
        else
        {
            move(LINES - 4,  COLS - 21);
            clrtoeol();
            mvaddstr(LINES - 4, COLS - 21, player.playerToolName().c_str());

            if(player.hasTools())
            {
                move(LINES - 5,  COLS - 21);
                clrtoeol();
                mvaddstr(LINES - 5, COLS - 21, notMatch.c_str());
            }
        }

        ch = getch();

        switch(ch)
        {
            case 't':
                {
                    player.toggleTool();
                    break;
                }

            case 'u':
                rating = player.useTool(obstacle);

                if(rating < 0)
                {
                    rating = 0;
                    break;
                }
                // update obstacle cost after using tool
                obstacleCost = obstacleCost / rating;
                toolUsed = true;
                break;

            case 'f':
                toolUsed = true; // a tool actually isn't being used here but player
                break;           // chose not to use a tool and this will exit the loop
        }
    }

    if(player.getEnergy() >= obstacleCost)
    {
        player.modifyEnergy(-obstacleCost);
        return true;
    }
                
    player.modifyEnergy(-obstacleCost);
    return false;
}
    

    
