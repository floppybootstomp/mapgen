#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include "../headers/globalVars.h"
#include "../headers/draw.h"
#include "../headers/dungeonmaker.h"
#include <curses.h>
#include <dirent.h>

#define MENU_MODE 0
#define LOAD_MODE 1
#define QUIT_MODE 2

// resets the menu to MENU_MODE
void resetMenuMode(int* menuSelector, int* ch, std::vector<std::string>* menuItems, int* mode)
{
    // reset menu
    *menuSelector = 0;
    *ch = -1;
    *menuItems = {"New map", "Save Map", "Load Map", "Quit"};
    *mode = MENU_MODE;

    // reset screen
    clear();
}

int main()
{
    initscr();
    setlocale(LC_CTYPE, "");
    curs_set(0);
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    Draw* d = new Draw();
    int sz = 64;
    DungeonMaker dungeon(sz, sz/2, sz*3, 3);

    clear();

    move(20, 20);
    refresh();

    std::string consoleText = "";
    std::string errorText = "";
    std::string loadFile = "";
    std::vector<std::string> menuItems;

    int ch;
    int mode;
    int menuSize;
    int menuSelector;

    resetMenuMode(&menuSelector, &ch, &menuItems, &mode);

    while(mode != QUIT_MODE)
    {
        // load a dungeon
        if(loadFile.size() > 0 && mode == MENU_MODE){
            std::string filename = "./dungeons/" + loadFile;
            int err = dungeon.loadDungeon(filename);

            if(err == 1)
            {
                loadFile = "";
                errorText = "Could not load dungeon: " + filename;
            }
            else
            {
                loadFile = "";
                consoleText = "Loaded Dungeon: " + filename;
            }
                
        }

        // console and error prompts
        d->drawString(stdscr, sz/2, 0, consoleText, HIGHLIGHT_PALLET);
        d->drawString(stdscr, sz/2, 0, errorText, COLOR_PALLET);

        // menu for managing maps
        menuSize = (int)menuItems.size();

        d->drawMenu(menuItems, sz/2, menuSize, menuSelector);
        dungeon.drawDungeon(d);

        // get user input
        ch = wgetch(stdscr);

        // reset console text
        if(consoleText.size() > 0 || errorText.size() > 0)
        {
            consoleText = "";
            errorText = "";
            clear();
        }

        // load variables
        DIR* dungeonDirectory;
        std::string fileNames;
        int i;
        dirent* fileEntry;

        switch(ch)
        {
            // move up menu
            case 'w':
            case 'a':
            case 'k':
            case 'h':
            case KEY_UP:
            case KEY_LEFT:
                if(menuSelector > 0)
                    menuSelector --;
                else
                    menuSelector = menuSize-1;
                break;
            // move down menu
            case 's':
            case 'd':
            case 'j':
            case 'l':
            case KEY_DOWN:
            case KEY_RIGHT:
                menuSelector = (menuSelector + 1) % menuSize;
                break;
            case 'b':
                if(mode == LOAD_MODE)
                    resetMenuMode(&menuSelector, &ch, &menuItems, &mode);
                break;
            case 10:
                int e;
                if(mode == MENU_MODE)
                {
                    switch(menuSelector)
                    {
                        case 0:
                            // generate a new dungeon
                            dungeon.resetDungeon();
                            break;
                        case 1:
                            // save current dungeon
                            e = dungeon.saveDungeon();

                            if(e)
                                errorText = "Could not save dungeon";
                            else
                                consoleText = "Saved Dungeon";

                            break;
                        case 2:
                            mode = LOAD_MODE;
                            dungeonDirectory = opendir("./dungeons");
                            fileNames = "";
                            i = 0;
                            fileEntry = readdir(dungeonDirectory);

                            // reset menuItems
                            menuItems.clear();
                            menuSelector = 0;

                            // loop through file names and add to menuItems
                            while(fileEntry != nullptr)
                            {
                                fileNames = fileEntry->d_name;
                                if(fileNames != "." && fileNames != ".."){
                                    menuItems.push_back(fileNames);
                                    i ++;
                                }
                                fileEntry = readdir(dungeonDirectory);
                            }
                            (void)closedir(dungeonDirectory);
                            ch = -1;

                            // reset screen
                            clear();

                            break;
                        case 3:
                            // close
                            mode = 2;
                            break;
                    }
                }
                if(mode == LOAD_MODE && ch == 10)
                {
                    // load dungeon
                    loadFile = menuItems[menuSelector];

                    resetMenuMode(&menuSelector, &ch, &menuItems, &mode);

                    break;
                }

                break;
            case 'q':
                mode = 2;
                break;
        }

        refresh();
    }

    endwin();
    return 0;
}
