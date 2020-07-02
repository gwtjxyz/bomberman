#include "CGameEntityBomb.hpp"

CGameEntityBomb::CGameEntityBomb(const int & x, const int & y, const char & display, const CGameEntity * owner) {
    m_X = x;
    m_Y = y;
    m_Display = display;
    m_SetBy = owner->getDisplay();
    m_Radius = owner->getAttribute(ATTR_POW);
    m_Placed = std::chrono::high_resolution_clock::now();
}

int CGameEntityBomb::getAttribute(const int & attrKey) const {
    //using if/else instead of switch so we can define time points only when we need to
    //and not every time this method is called
    if (attrKey == ATTR_SET_BY) {
        return m_SetBy;
    } else
    if (attrKey == ATTR_RADIUS) {
        return m_Radius;
    } else 
    if (attrKey == ATTR_TIME_PLACED) {
        auto temp = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = temp - m_Placed;
        if (duration.count() >= std::chrono::seconds(3).count()) {
            return ATTR_DETONATE;
        } else {
            return ATTR_NO_DETONATE;
        }
    } else {
        return ATTR_NONEXISTENT;
    }
}