#include "ui.h"
#include "display.h"
#include "player.h"
#include "camera.h"
#include "map.h"
#include "item.h"
#include "items/binoculars.h"
#include "items/chest.h"
#include "items/clue.h"
#include "items/diamond.h"
#include "items/food.h"
#include "items/obstacle.h"
#include "items/ship.h"
#include "items/tool.h"
#include <ncurses.h>
#include <iostream>

void UI::mainMenu(Display& display, bool& gameRunning, uint32_t& seed)
{
    nodelay(stdscr, false);
    clear();
    while(true)
    {
        display.printCenteredText(0, COLS, 8 , "Press number to select option");
        display.printCenteredText(0, COLS, 10, "1). Start Game");
        display.printCenteredText(0, COLS, 12, "2). Select Map Seed");
        display.printCenteredText(0, COLS, 14, "3). Quit Game");

        refresh();
        int input = getch();

        switch(input)
        {
            case 49: // 1 
            return;

            case 50: // 2
            seed = seedSelection(display, seed);
            break;

            case 51: // 3
            gameRunning = false;
            return;

            default:
            display.printCenteredText(0, COLS, 16, "Not a supported option, try a one of the number keys!");
        }
    }
}

/*void UI::printMapSave(Display& display, const Player& player, const Camera& camera, Map& map)
{
    printOutline(display, camera);
    auto [cx, cy] = camera.getDims();

    // Offset for all printed text in UI
    int xOffset = cx - cols + 2;

    std::string saveMap = "To Save Map: 'S'"; //should return user input back to saveMap
    mvaddstr(cy-8, xOffset, saveMap.c_str()); //map save 
}*/

uint32_t UI::seedSelection(Display& display, uint32_t currentSeed)
{
    clear();
    std::string seedText = "Current Seed: ";
    std::string seedStr;
    const int blen = 80;
    char buffer[blen];
    //int seedTextAdj;
    int curx = 0;

    bool exit = false;
    while(!exit)
    {
        refresh();

        display.printCenteredText(0, COLS, 8,  seedText + std::to_string(currentSeed));
        display.printCenteredText(0, COLS, 10, "1). Set Seed");
        display.printCenteredText(0, COLS, 12, "2). Exit to Main Menu: ");

        int ch = getch();

        switch(ch)
        {
            // Set seed
            case 49: // 1 
            echo();
            display.printCenteredText(0, COLS, 8,  seedText + std::to_string(currentSeed));
            curx = getcurx(stdscr);
            move(8,  curx - seedStr.length()-1);
            clrtoeol(); // clear to end of line
            getstr(buffer);
            seedStr = std::string{buffer};
            for(int i = 0; i < static_cast<int>(seedStr.length()); ++i)
                if(!std::isdigit(seedStr[i]))
                {
                    display.printCenteredText(0, COLS, 14, "Non-numeric entry! Try Again!");
                    goto Done;
                }

            currentSeed = std::stoi(seedStr);
            Done:
            noecho();
            clear();
            break;
            
            case 50: // 2
            exit = true;
            break;

            default:
            display.printCenteredText(0, COLS, 14, "Not a supported option, try a one of the number keys!");
        }
    }
    clear();
    return currentSeed;
}

void UI::print(Display& display, const Player& player, const Camera& camera, Map& map)
{
    printOutline(display, camera);
    auto [cx, cy] = camera.getDims();

    // Offset for all printed text in UI
    int xOffset = cx - cols + 2;

    mvaddstr(5, xOffset, "Options:");
    mvaddstr(6, xOffset, "1) North");
    mvaddstr(7, xOffset, "2) East");
    mvaddstr(8, xOffset, "3) South");
    mvaddstr(9, xOffset, "4) West");
    

    std::string curTool = "Current Tool (T):";
    std::string tool = player.playerToolName();
    std::string wifs = "Whiffles: " + std::to_string(player.getMoney());
    std::string ener = "Energy:   " + std::to_string(player.getEnergy());
    

    mvaddstr(cy-5, xOffset, curTool.c_str());
    mvaddstr(cy-4, xOffset, tool.c_str());
    mvaddstr(cy-3, xOffset, wifs.c_str());
    mvaddstr(cy-2, xOffset, ener.c_str());

    printSelectedInfo(player, map, camera, xOffset);
}

void UI::printOutline(Display& display, const Camera& camera)
{
    auto [cx, cy] = camera.getDims();

    int xpos = cx - cols;

    move(0, xpos);
    vline('#', 128);
}

void UI::printSelectedInfo(const Player& player, Map& map, const Camera& camera, int xOffset)
{

    auto [cx, cy]   = player.getCursor();
    auto [cxo, cyo] = camera.getOffsets();
    curs_set(1);

    move(cy - cyo, cx - cxo);
    const MapSquare& sq =  map.sq(cx, cy);

    if(!sq.item)
        return;
    
    if(sq.discovered && sq.item)
    {
        if (dynamic_cast<Clue*>(sq.item))
        {
            // Need to update the clue's second statement
            Map* mp = &map;
            Clue* c = dynamic_cast<Clue*>(sq.item);
            c->setDescription(mp);

            // Clues are displayed differently
            mvaddstr(1, xOffset, "Clue Found!!");
            refresh();
            printWindow(sq.item);
        }
        else
        {
            // TODO: Add all other item types in here
            auto [l1, l2, l3, l4] = sq.item->getDescription();
            mvaddstr(1, xOffset, l1.c_str());
            mvaddstr(2, xOffset, l2.c_str());
            mvaddstr(3, xOffset, l3.c_str());
            mvaddstr(4, xOffset, l4.c_str());
        }
        curs_set(0);
    }
    return; 
}

void UI::printWindow(Item* item)
{
    auto [l1, l2, l3, l4] = item->getDescription();
    std::string s2 = l2.c_str();
    std::string s3 = l3.c_str();
    std::string s4 = l4.c_str();


    int len = s2.length();
    len = std::max(len, (int)s3.length());
    len = std::max(len, (int)s4.length());

    int h = 10;
    int o = 2;
    int w = len+o+2;

    WINDOW* win = newwin(h,w,(LINES-h)/2,(COLS-w)/2);

    // Draw a border
    mvwvline(win,0,  0,  '*',h);
    mvwvline(win,0,  w-1,'*',h);
    mvwhline(win,0,  0,  '*',w);
    mvwhline(win,h-1,0,  '*',w);

    // Print info
    mvwprintw(win, 1, o, "The clue reads...");
    mvwaddstr(win, 3, o, l2.c_str());
    mvwaddstr(win, 5, o, l3.c_str());
    mvwaddstr(win, 7, o, l4.c_str());

    wrefresh(win);
    delwin(win);
}