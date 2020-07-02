#pragma once

#include <chrono>

#include "CGameEntity.hpp"
#include "CConstants.hpp"
/**
* Bomb variant of an entity; has attributes radius, set by (who the owner is for remote detonation), and time placed
*/
class CGameEntityBomb: public CGameEntity {
public:
    /**
     * initializes position, display character and owner
     * @param[in] pos x
     * @param[in] pos y
     * @param[in] display character
     * @param[in] who placed it
     */
    CGameEntityBomb(const int & x, const int & y, const char & display, const CGameEntity * owner);

    void moveX(const int &) {return;}

    void moveY(const int &) {return;}
    /**
     * does absolutely nothing and
     * @returns false
     */
    bool modify(const int &, const int &) override {return false;} 
    /**
     * @returns attributes based on given key if present, an ATTR_NONEXISTENT value otherwise
     */
    int getAttribute(const int & attrKey) const override;

protected:
    //time point for calculating whether or not it's time for the bomb to explode
    std::chrono::high_resolution_clock::time_point m_Placed;

    int m_Radius;

    char m_SetBy;
};