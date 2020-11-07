#include "input.h"
#include <ncurses.h>
#include "player.h"
#include "map.h"

// Returns false if exit key is pressed. Handles all input
bool Input::input(Player& player, Map& map)
{
    int ch = getch();

    if(mode == NORMAL)
        switch(ch)
        {
            case 'q':
                return false;

            case 'i':
                mode = UI;
                break;

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
        }
    else
        switch (ch)
        {
        case 'q':
            mode = NORMAL;
            break;
        
        default:
            break;
        }

    return true;
}