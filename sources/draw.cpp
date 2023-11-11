#include "../headers/globalVars.h"
#include "../headers/draw.h"
#include <iostream>

Draw::Draw()
{
    initColors();
}

Draw::~Draw()
{
}

void Draw::initColors()
{
    if(!has_colors())
    {
        endwin();
        std::cout << "Your terminal does not support color" << std::endl;
        exit(1);
    }

    start_color();
    init_pair(LIGHT_PALLET, COLOR_BLACK, COLOR_WHITE);
    init_pair(DARK_PALLET, COLOR_WHITE, COLOR_BLUE);
    init_pair(BACKGROUND_PALLET, COLOR_WHITE, COLOR_BLACK);
    init_pair(HIGHLIGHT_PALLET, COLOR_BLUE, COLOR_YELLOW);
    init_pair(COLOR_PALLET, COLOR_RED, COLOR_BLACK);
}

void Draw::drawString(WINDOW *win, int startX, int startY, std::string c, int colorPallet)
{
    if(win == nullptr)
        win = stdscr;

    attron(COLOR_PAIR(colorPallet));
    mvwprintw(win, startX, startY, "%s", c.c_str());
	attroff(COLOR_PAIR(colorPallet));
	refresh();
}
