#pragma once

#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "CConstants.hpp"

/**
 * Class for loading a level from a file into memory and rewriting its high score
 * designed for reading simple .txt files so the user could have an easier time 
 * creating and saving levels but it could work with just about any format storing
 * ASCII characters
 */
class CLevelBuilder {
public:
    /**
     * fills out the m_Level container if the file provided is valid; returns true upon success
     * 
     * @param[in] string with the level file name
     * @returns true if building level was successful, false otherwise
     */
    bool buildLevel (const std::string & levelName);
    /**
     * @returns the m_Level container
     */
    std::map<std::pair<int,int>,char> getLevel() {return m_Level;}
    /**
     * @returns the container with player spawnpoints
     */
    std::vector<std::pair<int, int>> getPlayerSpawnpoints() {return m_PlayerSpawnpoints;}
    /**
     * @returns the container with NPC spawnpoints
     */
    std::vector<std::pair<int, int>> getNPCSpawnpoints() {return m_NPCSpawnpoints;}
    /**
     * @returns the high score value
     */
    int getHighScore() {return m_HighScore;}
    /**
     * changes the high score of the level
     * 
     * @param[in] high score input value 
     */
    void setHighScore(const int & highScore);
    /**
     * returns level dimensions through paremeters
     * 
     * @param[in] size x
     * @param[in] size y
     */
    void getLevelDimensions(int & x, int & y);
    /**
     * what do you think this one does
     * 
     * @param[in] file stream to close before returning false
     * @returns true, probably
     */
    bool closeFileStreamAndReturnFalse(std::fstream & fs);

private:
    //container for the level map
    std::map<std::pair<int,int>,char> m_Level;
    //container for player spawnpoints
    std::vector<std::pair<int, int>> m_PlayerSpawnpoints;
    //container for npc spawnpoints
    std::vector<std::pair<int, int>> m_NPCSpawnpoints;
    //string holding the file name
    std::string m_LevelName;
    
    int m_HighScore;

    int m_Xmax;

    int m_Ymax;
};