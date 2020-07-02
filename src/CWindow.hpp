#pragma once

#include <string>
#include <vector>
#include "ncurses.h"

/**
 * Helper class allowing easier management of windows
 */

class CWindow {
public:
    /**
     * initializes and returns a window of given width/height placed in given position
     * 
     * @param[in] window height
     * @param[in] window width
     * @param[in] window starting y coordinates
     * @param[in] window starting x coordinates
     * 
     * @returns pointer to the created window
     */
    WINDOW * createWindow(const int & height, const int & width, const int & starty, const int & startx);
    /**
     * prints a menu based on a string of options given to it
     * 
     * @param[in] window pointer
     * @param[in] highlight value
     * @param[in] container with options for the menu
     * @param[in] x position for printing
     * @param[in] y position for printing
     */
    void printMenu(WINDOW * menu_win, const int & highlight, const std::vector<std::string> & choices, const int & srcx, const int & srcy);
    /**
     * changes the borders of a window
     * 
     * @param[in] window pointer
     * @param[in] left side/right side border
     * @param[in] top side/bottom side border
     * @param[in] corner symbol
     */
    void setWindowBorder(WINDOW * win, const char & lsrs, const char & tsbs, const char & corner);
    /**
     * clears a window from a screen and frees allocated memory
     * 
     * @param[in] window to be destroyed
     */
    void destroyWindow(WINDOW * win);
};