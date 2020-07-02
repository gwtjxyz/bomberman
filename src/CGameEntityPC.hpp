#pragma once

#include "CGameEntity.hpp"

#include <chrono>

/**
 * player variant of an entity; has attributes HP, power (blast radius), bomb counter
 * (goes up and down as bombs are placed and exploded), max bomb counter, remote
 * detonation ability, score, and a point in time at which they last took damage
 */
class CGameEntityPC: public CGameEntity {
public:
    /**
     * initializes the player entity
     * 
     * @param[in] pos x
     * @param[in] pos y
     * @param[in] display character
     */
    CGameEntityPC(int & x, int & y, char display);

    void moveX(const int & amt) override;

    void moveY(const int & amt) override;
    /**
     * modifies attributes based on given key if present and 
     * @returns true if attributes were found and modified, false otherwise
     */
    bool modify(const int & key, const int & amt) override;
    /**
     * @returns attributes based on given key if present; ATTR_NONEXISTENT otherwise
     */
    int getAttribute(const int & attrKey) const override;


protected:
    int m_HP = 1;
    int m_Pow = 1;
    int m_BombCnt = 0;
    int m_MaxBombCnt = 1;
    int m_HasRemote = 0;
    int m_Score = 0;
    //last time the player took damage for calculating invulnerability timer
    std::chrono::high_resolution_clock::time_point m_LastTakenDamage;
};