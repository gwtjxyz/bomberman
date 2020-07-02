#include "CWindow.hpp"

WINDOW * CWindow::createWindow(const int & height, const int & width, const int & starty, const int & startx) {
    WINDOW * win;

    win = newwin(height, width, starty, startx);
    box(win, 0 , 0);
    wrefresh(win);

    return win;
}

void CWindow::printMenu(WINDOW * menu_win, const int & highlight, const std::vector<std::string> & choices, const int & srcx = 2, const int & srcy = 2) {
    int y = srcy;
    for(int i = 0; i != (int)choices.size(); ++i) {	
        if(highlight == i + 1) { //highlight current choice
            wattron(menu_win, A_REVERSE); 
            mvwprintw(menu_win, y, srcx, "%s", choices[i].c_str());
            wattroff(menu_win, A_REVERSE);
        }
        else {
            mvwprintw(menu_win, y, srcx, "%s", choices[i].c_str());
        }
        y += 2;
    }
    wrefresh(menu_win);
}

void CWindow::setWindowBorder(WINDOW * win, const char & lsrs, const char & tsbs, const char & corner) {
    wborder(win, lsrs, lsrs, tsbs, tsbs, corner, corner, corner, corner);
}

void CWindow::destroyWindow(WINDOW * win) {	
    wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    werase(win);
    wrefresh(win);
    delwin(win);
}