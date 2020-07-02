#pragma once

#include "CGameEntity.hpp"

/**
 * NPC variant of an entity; has HP and direction attributes
 * 
 */

class CGameEntityNPC : public CGameEntity {
public:
    /**
     * initializes position and display character
     * 
     * @param[in] pos x
     * @param[in] pos y
     * @param[in] display character
     */
    CGameEntityNPC(const int & x, const int & y, const char & display);

    void moveX(const int & amt) override;

    void moveY(const int & amt) override;

    bool modify(const int & key, const int & amt) override;

    int getAttribute(const int & attrKey) const override;

protected:
    int m_HP = 1;
    //direction the entity last moved in; for AI
    int m_Direction = 0;
};