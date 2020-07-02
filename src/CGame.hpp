#pragma once

#include <map>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <thread>
#include <set>
#include "ncurses.h"

#include "CConstants.hpp"
#include "CLevelBuilder.hpp"
#include "CGameEntityPC.hpp"
#include "CGameEntityNPC.hpp"
#include "CGameEntityBomb.hpp"


/**
 * This class handles all the game logic and calculations as well as the rendering
 * of the game field onto the screen; the status text rendering is handled by CMenu instead.
 */
class CGame {
public:
    /**
     * Frees dynamically allocated CGameEntity elements
     */
    ~CGame();
    /**
     * Sets up an array of entities depending on the game state and assigns a level map and a game mode value
     * to the class.
     * 
     * @param[in] game window pointer
     * @param[in] CLevelBuilder class instance
     * @param[in] game mode value
     */
    void initializeGame(WINDOW * srcWin, CLevelBuilder & levelBuilder, const int & gameMode);
    /**
     * master function for all the relevant game calculations;
     * 
     * 
     * @param[in] player 1 state boolean
     * @param[in] player 2 state boolean
     * @returns different values depending on the game state
     */
    int gameLoop(bool & player1Alive, bool & player2Alive);
    /**
     * reads from a container holding user inputs and does appropriate actions
     * @returns false if game exit key was pressed, true otherwise
     */
    bool processInput();
    /**
     *draws the level tiles
     */
    void drawLevel();       
    /**
     *draws entities on top of the level map
     */
    void drawEntities();
    /**
     *draws the explosion map container on top of the level map and entities
     */
    void drawExplosionOverlay();
    /**
     * clears the explosion overlay map
     */
    void clearExplosionOverlay();
    /**
     * generic X axis move function; returns false upon unsuccessful move attempt
     * @param[in] entity being moved
     * @param[in] direction of movement
     * @returns true if movement was successful, false otherwise
     */
    bool moveX(CGameEntity * entity, const int & amt);
    /**
     * generic Y axis move function; returns false upon unsuccessful move attempt
     * @param[in] entity being moved
     * @param[in] direction of movement
     * @returns true if movement was successful, false otherwise
     */
    bool moveY(CGameEntity * entity, const int & amt);
    /**
     *  moves entities based on their internal direction (which only NPCs have)
     *  in case moving fails, randomize direction until moving succeeds
     */
    void moveNPCs();
    /**
     *deal damage to players colliding with NPCs
     */
    void attackPlayer();
    /**
     * check if any entities are dead and remove them from the entity container;
     * if said dead entity is player or if said player is the only entity left alive, figure out game state (loss/win)
     * 
     * @param[in] player 1 state boolean
     * @param[in] player 2 state boolean
     * 
     * @returns true if only 1 player remains in the container (singleplayer)
     */
    bool clearDeadEntities(bool & player1Alive, bool & player2Alive); 
    /**
     * creates a CGameEntityBomb instance inside the entities container
     * 
     * @param[in] player entity who is assigned as the bomb's long lost father
     */
    void placeBomb(CGameEntity * player);
    /**
     * tests bomb explosion timers and explodes bombs where relevant
     */
    void explodeBombs();
    /**
     * explodes the last placed bomb by the player
     * 
     * @param[in] player doing the exploding
     */
    void explodeBombs(const char & player);
    /**
     * calculate where an explosion should be based on initial coords + blast radius
     * 
     * @param[in] iterator pointing to the bomb being exploded
     */
    void calculateExplosion(std::vector<CGameEntity *>::iterator it);
    /**
     * calculate and draw an explosion at a tile while also applying damage where relevant
     * based on the position fed to it through arguments;
     * 
     * @param[in] X pos
     * @param[in] Y pos
     * @eturns true if explosion should continue going in the direction it was going in,
     * false otherwise (if it hits a breakable/unbreakable wall tile)
     */
    bool createExplosionOverlay(const int & posX, const int & posY);
    /**
     *  figure out which powerups are enabled and fill the powerup container accordingly
     */
    void configurePowerups();
    /**
     * roll for what powerup to apply to the player by going through the powerup container
     * 
     * @param[in] player being given the powerup
     */
    void calculatePowerUp(CGameEntity * player);
    /**
     *  check if there are any player entities to apply powerups to
     */
    void applyPowerUps();
    /**
     * extracts high score if requested
     * @returns high score
     */
    int getScore() const;
    //container for players/NPCs/bombs made public to be able to be accessed by classes like CMenu
    //for displaying game status
    std::vector<CGameEntity *> m_Entities;

private:
    //data structure for gathering user inputs
    std::set<int> m_Inputs;
    //points to a window allocated by CMenu - don't need to free as a result
    WINDOW * m_GameWindow;
    //data structure for holding tile level data
    std::map<std::pair<int,int>,char> m_Level;
    //data structure for displaying bomb explosions
    std::map<std::pair<int,int>,char> m_ExplosionOverlay;
    //data structure for holding possible powerups
    std::vector<int> m_PowerUpList;
    //level dimension
    int m_Xmax;
    //level dimension
    int m_Ymax;
    //integer for determining when to do AI movement
    int m_AIDelay;
    //integer for storing user inputs before inserting them into a set
    int m_Input;
    //game state integer for determining gamemode-related game logic
    int m_GameMode;

};