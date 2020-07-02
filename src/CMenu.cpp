#include "CMenu.hpp"

CMenu::CMenu() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    m_WindowsInitialized = false;
    //creates a menu window in the center of the screen
    WINDOW * win = m_WinMgr.createWindow(MENU_HEIGHT, MENU_WIDTH, (LINES - MENU_HEIGHT) / 2, (COLS - MENU_WIDTH) / 2);
    keypad(win, true);
    m_Windows.push_back(win);
}

CMenu::~CMenu() {
    //clears the window container
    for (auto it : m_Windows) {
        m_WinMgr.destroyWindow(it);
    }
    endwin();
}

void CMenu::mainMenu() {
    //menuState determines where in the menu hierarchy we are; we're using
    //predefined constants for easier navigation
    int menuState = MENU_STATE_MAIN;
    int selection = 0;
    std::string fileName(50, '\0');
    std::vector<std::string> menuItems;

    menuItems.push_back("1 PLAYER");
    menuItems.push_back("2 PLAYERS");
    menuItems.push_back("EXIT");

    int score = -1;

    while(menuState != MENU_STATE_EXIT) {
        werase(m_Windows[0]);
        box(m_Windows[0], 0, 0);
        //m_WinMgr.setWindowBorder(m_Windows[0], '|', '-', '+');
        
        switch(menuState) {
            case (MENU_STATE_MAIN):
                //displays the main menu and stuff
                mvwprintw(m_Windows[0], 1, 1, "Hello and welcome to Bomberman!");
                mvwprintw(m_Windows[0], 2, 1, "Use the arrow keys to navigate the main menu.");
                refresh();
                selection = printSelectionMenu(0, menuItems, 2, 4);
                refresh();
                switch(selection) {
                    case 1:
                        m_GameMode = GAMEMODE_SINGLEPLAYER;
                        menuState = MENU_STATE_LEVELSELECT;
                        break;
                    case 2: //bugged in the version i had sent WHOOPS
                        m_GameMode = GAMEMODE_MULTIPLAYER;
                        menuState = MENU_STATE_LEVELSELECT;
                        break;
                    case 3:
                        menuState = MENU_STATE_EXIT;
                        break;
                }
                break;
            case (MENU_STATE_LEVELSELECT):
                //asks the user to input a file name
                mvwprintw(m_Windows[0], 1, 1, "Please input the level file name, or type");
                mvwprintw(m_Windows[0], 2, 1, "\"back\" to go back.");
                curs_set(1);
                keypad(m_Windows[0], false);
                echo();
                refresh();
                mvwscanw(m_Windows[0], 4, 1, "%s", fileName.c_str());
                refresh();
                curs_set(0);
                noecho();
                keypad(m_Windows[0], true);
                //we allow people to use caps lock and not get punished for it cause we're nice
                std::transform(fileName.begin(),fileName.end(),fileName.begin(), ::tolower);
                if (!strcmp(fileName.c_str(),"back\0")) {
                    menuState = 0;
                } else {
                    if (!processInput(fileName)) {
                        mvwprintw(m_Windows[0], 6, 1, "Error: Corrupted or nonexistent file!");
                        mvwprintw(m_Windows[0], 7, 1, "Please try again. Press any key to continue...");
                        refresh();
                        wgetch(m_Windows[0]);
                    } else {
                        menuState = 2;
                    }
                }
                break;
            case (MENU_STATE_RUNGAME):
                //get the menu window out of the way and run the game logic code
                wclear(m_Windows[0]);
                wrefresh(m_Windows[0]);
                refresh();
                score = runGame(m_GameMode);
                m_Level.setHighScore(score);
                destroyWindows();
                menuState = MENU_STATE_MAIN;
                break;
            default:
                break;
        }
    }
    return;
}

int CMenu::printSelectionMenu(const int & windowIndex, const std::vector<std::string> & menuItems, const int & posX, const int & posY) {
    int selection = 0;
    int highlight = 1;
    int ch;

    m_WinMgr.printMenu(m_Windows[windowIndex], highlight, menuItems, posX, posY);
    refresh();

    while(1){
        ch = wgetch(m_Windows[0]);
        switch(ch) {
            case KEY_UP:
                if (highlight == 1) {
                    highlight = menuItems.size();
                } else {
                    --highlight;
                }
                break;
            case KEY_DOWN:
                if (highlight == (int)menuItems.size()) {
                    highlight = 1;
                } else {
                    ++highlight;
                }
                break;
            case 10:
                selection = highlight;
                break;
            default:
                break;
        }
        m_WinMgr.printMenu(m_Windows[windowIndex], highlight, menuItems, posX, posY);

        if (selection != 0)
            return selection;
        
        refresh();
    }
}

bool CMenu::processInput(std::string & input) {
    std::string fileName = "examples/";
    fileName.append(input);
    return m_Level.buildLevel(fileName);
}

void CMenu::initializeWindows() {
    int dimX, dimY;
    m_Level.getLevelDimensions(dimX, dimY);
    int offsetX = (COLS - (dimX + 2)) / 2;  //centers X axis
    WINDOW * gameWin = m_WinMgr.createWindow(dimY + 2, dimX + 2, 0, offsetX);
    keypad(gameWin, true);
    m_Windows.push_back(gameWin);   //game window; index 1
    //displays the status display
    WINDOW * statusWin = m_WinMgr.createWindow(STATUS_WINDOW_HEIGHT, STATUS_WINDOW_WIDTH, dimY + 2, ((COLS - STATUS_WINDOW_WIDTH) / 2));
    m_Windows.push_back(statusWin); //status display; index 2
    m_WindowsInitialized = true;
}

void CMenu::destroyWindows() {
    if (m_WindowsInitialized) {
        m_WindowsInitialized = false;
        m_WinMgr.destroyWindow(m_Windows[2]);
        m_Windows.pop_back();
        m_WinMgr.destroyWindow(m_Windows[1]);
        m_Windows.pop_back();
    }
}

int CMenu::runGame(const int & gameMode) {
    CGame game;
    
    initializeWindows();

    game.initializeGame(m_Windows[1], m_Level, gameMode);

    bool player1Alive = true, player2Alive = true;

    int gameState = 1;

    while (player1Alive && player2Alive) {
        gameState = game.gameLoop(player1Alive, player2Alive);
        if (gameState != GAME_CONTINUE)
            break;
        displayStatus(game, m_Windows[2]);
    }

    if (gameState == GAME_QUIT_BY_PLAYER) {
        endScreen(m_Windows[2]);
        return GAME_QUIT_BY_PLAYER;
    }

    if (gameMode == GAMEMODE_SINGLEPLAYER) {
        endScreen(m_Windows[2], player1Alive, game.getScore());
        return game.getScore();
    }
    else {
        endScreen(m_Windows[2], player1Alive, player2Alive);
        return ATTR_NONEXISTENT;
    }
}

void CMenu::displayStatus(CGame & game, WINDOW * statusWin) {
    wclear(statusWin);
    box(statusWin, 0, 0);

    mvwprintw(statusWin, 1, 1, "P1: %02dHP %02dPOW %02dBMB ", 
              game.m_Entities[0]->getAttribute(ATTR_HP),
              game.m_Entities[0]->getAttribute(ATTR_POW),
              game.m_Entities[0]->getAttribute(ATTR_MAX_BOMB_CNT));

    if (game.m_Entities[0]->getAttribute(ATTR_REMOTE))
        waddstr(statusWin, "REMOTE");
    
    if (m_GameMode == GAMEMODE_MULTIPLAYER) {
        mvwprintw(statusWin, 3, 1, "P2: %02dHP %02dPOW %02dBMB ", 
                game.m_Entities[1]->getAttribute(ATTR_HP),
                game.m_Entities[1]->getAttribute(ATTR_POW),
                game.m_Entities[1]->getAttribute(ATTR_MAX_BOMB_CNT));

        if (game.m_Entities[1]->getAttribute(ATTR_REMOTE))
            waddstr(statusWin, "REMOTE");
    }
    mvwprintw(statusWin, 5, 1, "Press %c to quit", GAME_QUIT);
    wrefresh(statusWin);
}

void CMenu::endScreen(WINDOW * statusWin) {
    wclear(statusWin);
    box(statusWin, 0, 0);

    mvwprintw(statusWin, 1, 1, "Thanks for playing!");
    endScreenDelay(statusWin);
}

void CMenu::endScreen(WINDOW * statusWin, const bool & player1Alive, const int & score) {
    wclear(statusWin);
    box(statusWin, 0, 0);

    if (!player1Alive)
        mvwprintw(statusWin, 1, 1, "GAME OVER! Score: %d", score);
    else
        mvwprintw(statusWin, 1, 1, "VICTORY!!! Score: %d", score);
    endScreenDelay(statusWin);
}

void CMenu::endScreen(WINDOW * statusWin, const bool & player1Alive, const bool & player2Alive) {
    wclear(statusWin);
    box(statusWin, 0, 0);
    if (player1Alive) {
        mvwprintw(statusWin, 1, 1, "PLAYER 1 VICTORIOUS!");
    }
    if (player2Alive) {
        mvwprintw(statusWin, 1, 1, "PLAYER 2 VICTORIOUS!");
    } 
    if (!player1Alive && !player2Alive) {
        mvwprintw(statusWin, 1, 1, "You both lost! Somehow...");
    }
    endScreenDelay(statusWin);
}

void CMenu::endScreenDelay(WINDOW * statusWin) {
    wrefresh(statusWin);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    mvwprintw(statusWin, 2, 1, "Press any key to continue...");
    flushinp();
    wgetch(statusWin);
    clear();
    refresh();
}

