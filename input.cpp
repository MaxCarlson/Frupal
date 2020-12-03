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
    int toolHelp = 0;

    move(LINES - 4,  COLS - 21);
    clrtoeol(); // clear to end of line
    std::string tool = player.compatibleTools(obstacle);
    mvaddstr(LINES - 4, COLS - 21, tool.c_str());

    while(ch != 'd' || obstacleCost <= toolHelp)
    {
        ch = getch();

        switch(ch)
        {
            case 't':
                {
                    player.toggleTool();
                    move(LINES - 4,  COLS - 21);
                    clrtoeol(); // clear to end of line
                    //tool = player.playerToolName();
                    tool = player.compatibleTools(obstacle);
                    mvaddstr(LINES - 4, COLS - 21, tool.c_str());
                    move(LINES - 5,  COLS - 21);
                    clrtoeol(); // clear to end of line
                    tool = player.curToolIDX();
                    mvaddstr(LINES - 5, COLS - 21, tool.c_str());
                    break;
                }

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
                
    player.modifyEnergy(-obstacleCost);
    return false;
}
    

    
