#include "input.h"
#include <ncurses.h>


// Returns false if exit key is pressed. Handles all input
bool Input::input()
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
                
                break;

            case KEY_RIGHT:

                break;

            case KEY_UP:

                break;

            case KEY_DOWN:

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