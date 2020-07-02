#include "CGameEntityPC.hpp"

CGameEntityPC::CGameEntityPC(int & x, int & y, char display) {
        m_X = x;
        m_Y = y;
        m_Display = display;
        m_LastTakenDamage = std::chrono::high_resolution_clock::now();
    }

void CGameEntityPC::moveX(const int & amt) {m_X += amt;}

void CGameEntityPC::moveY(const int & amt) {m_Y += amt;}

bool CGameEntityPC::modify(const int & key, const int & amt) {
    auto temp = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = temp - m_LastTakenDamage;
        switch(key) {
            case (ATTR_DAMAGE):
                if (duration.count() >= std::chrono::seconds(INVULNERABILITY_TIMER).count()) {
                    m_HP -= amt;
                    m_Score -= SCORE_DAMAGE_PENALTY;
                    if (m_HP < 0)
                        m_HP = 0;
                    m_LastTakenDamage = temp;
                }
                break;
            case (ATTR_POW):
                m_Pow += amt;
                break;
            case (ATTR_MAX_BOMB_CNT):
                m_MaxBombCnt += amt;
                break;
            case (ATTR_BOMB_CNT):
                if (m_BombCnt == m_MaxBombCnt && amt > 0)
                    return false;
                m_BombCnt += amt;
                if (m_BombCnt < 0)
                    m_BombCnt = 0;
                break;
            case (ATTR_ONEUP):
                m_HP += amt;
                break;
            case (ATTR_REMOTE):
                if (m_HasRemote)
                    m_HasRemote = 0;
                else
                    m_HasRemote = 1;
                break;
            case (ATTR_SCORE):
                m_Score += amt;
                break;
            default:
                return false;
        }
        return true;
    }

int CGameEntityPC::getAttribute(const int & attrKey) const {
    switch (attrKey) {
        case (ATTR_HP):
            return m_HP;
        case (ATTR_POW):
            return m_Pow;
        case (ATTR_BOMB_CNT):
            return m_BombCnt;
        case (ATTR_MAX_BOMB_CNT):
            return m_MaxBombCnt;
        case (ATTR_REMOTE):
            return m_HasRemote;
        case (ATTR_SCORE):
            return m_Score;
        default:
            return ATTR_NONEXISTENT;
    }
}