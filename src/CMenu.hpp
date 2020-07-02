#pragma once

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cstring>
#include <chrono>
#include <thread>
#include "ncurses.h"

#include "CGame.hpp"
#include "CMenu.hpp"
#include "CLevelBuilder.hpp"
#include "CWindow.hpp"

/**
 * Class for handling the main menu of the game; besides that, processes
 * player inputs for selecting gamemodes and then handles the looping of the game logic and 
 * displaying game status/results of the game.
 */

class CMenu {
public:
    /**
     * creates the initial menu window
     */
    CMenu();
    /**
     * deallocates allocated memory
     */
    ~CMenu();
    /**
     * handles the main menu and calls other functions based on user choice
     */
    void mainMenu();
    /**
     * handles the display of a multiple option menu and returns the user's chosen item's numeric value
     * 
     * @param[in] window index to where to display the menu
     * @param[in] vector of items to be displayed in the menu
     * @param[in] pos x of the menu
     * @param[in] pos y of the menu
     */
    int printSelectionMenu(const int & windowIndex, 
                           const std::vector<std::string> & menuItems, 
                           const int & posX, const int & posY);
    /**
     * appends the folder path to the file name input by the user; returns false in case such a file was not found
     * or the file was determined to be in a bad format
     * 
     * @param[in] file name string
     * @returns true if file read was successful, false otherwise
     */
    bool processInput(std::string & input);
    /**
     * initializes windows for game and status display
     */
    void initializeWindows();
    /**
     * destroys and frees windows for game and status display
     */
    void destroyWindows();
    /**
     * handles the continuous run of the game loop inside CGame and what to display inside the status window
     * 
     * @param[in] game mode
     * @returns predefined constant indicating the outcome of the game
     */
    int runGame(const int & gameMode);
    /**
     * draws the status window
     * 
     * @param[in] CGame instance
     * @param[in] pointer to status window
     */
    void displayStatus(CGame & game, WINDOW * statusWin);
    /**
     * end screen version for single player game end
     * 
     * @param[in] status window
     * @param[in] playing 1 status boolean
     * @param[in] score achieved
     */
    void endScreen(WINDOW * statusWin, const bool & player1Alive, const int & score);
    /**
     * end screen version for multiplayer game end
     * 
     * @param[in] status window
     * @param[in] player 1 status boolean
     * @param[in] player 2 status boolean
     */
    void endScreen(WINDOW * statusWin, const bool & player1Alive, const bool & player2Alive);
    /**
     *  end screen version for user pressing the exit key
     * 
     * @param[in] status window
     */
    void endScreen(WINDOW * statusWin);
    /**
     * adds a short delay before allowing to close the end screen so the user doesnt accidentally close it
     * without intending to do so
     * 
     * @param[in] status window
     */
    void endScreenDelay(WINDOW * statusWin);

private:
    //program state variable for determining whether or not to call the allocators/destructors for windows
    bool m_WindowsInitialized;
    //CWindow helper class instance for easier window management
    CWindow m_WinMgr;
    //container for various windows (menu/game screen/game state display)
    std::vector<WINDOW *> m_Windows;
    //CLevelBuilder class instance for loading and modifying a level
    CLevelBuilder m_Level;
    //variable for storing the game mode chosen by the user
    int m_GameMode;
    //variable for storing who won
    int m_Winner;
};