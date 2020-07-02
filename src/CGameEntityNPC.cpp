#include "CGameEntityNPC.hpp"

CGameEntityNPC::CGameEntityNPC(const int & x, const int & y, const char & display) {
    m_X = x;
    m_Y = y;
    m_Display = display;
}

void CGameEntityNPC::moveX(const int & amt) {m_X += amt;}

void CGameEntityNPC::moveY(const int & amt) {m_Y += amt;}

bool CGameEntityNPC::modify(const int & key, const int & amt) {
    switch (key) {
        case (ATTR_DAMAGE):
            m_HP -= amt;
            if (m_HP < 0)
                m_HP = 0;
            break;
        case (ATTR_DIRECTION):
            m_Direction = amt;
            break;
        default:
            return false;
    }
    return true;
}

int CGameEntityNPC::getAttribute(const int & attrKey) const {
    switch(attrKey) {
        case (ATTR_HP):
            return m_HP;
        case (ATTR_DIRECTION):
            return m_Direction;
        default:
            return ATTR_NONEXISTENT;
    }
}