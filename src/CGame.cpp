#include "CGame.hpp"

CGame::~CGame() {
    for (auto it : m_Entities) {
        delete it;
    }
    m_Entities.clear();
}

void CGame::initializeGame(WINDOW * srcWin, CLevelBuilder & levelBuilder, const int & gameMode) {
    m_GameMode = gameMode;
    m_GameWindow = srcWin;
    //wclear(m_GameWindow);
    //wborder(m_GameWindow, 0, 0, 0, 0, 0, 0, 0, 0);
    m_AIDelay = AI_MOVEMENT_DELAY;
    nodelay(m_GameWindow, true);
    m_Level = levelBuilder.getLevel();
    levelBuilder.getLevelDimensions(m_Xmax, m_Ymax);

    std::vector<std::pair<int,int>> playerSpawnpoints = levelBuilder.getPlayerSpawnpoints();

    CGameEntityPC * player1 = new CGameEntityPC(playerSpawnpoints[0].first, 
                                                playerSpawnpoints[0].second,
                                                DISPLAY_PLAYER_1);

    m_Entities.push_back(player1);

    if (gameMode == GAMEMODE_MULTIPLAYER) {

        CGameEntityPC * player2 = new CGameEntityPC(playerSpawnpoints[1].first,
                                                    playerSpawnpoints[1].second,
                                                    DISPLAY_PLAYER_2);

        m_Entities.push_back(player2);
    } else {
        std::vector<std::pair<int,int>> NPCSpawnpoints = levelBuilder.getNPCSpawnpoints();
        for (auto it : NPCSpawnpoints) {
            CGameEntityNPC * NPC = new CGameEntityNPC(it.first, it.second, DISPLAY_NPC);
            NPC->modify(ATTR_DIRECTION, std::rand() % 4);
            m_Entities.push_back(NPC);
        }
    }

    if (DISABLE_ALL_POWERUPS == 0)
        configurePowerups();

    bool player1Alive = true, player2Alive = true;
    gameLoop(player1Alive, player2Alive);
    if (!player1Alive || !player2Alive) {
        mvwprintw(m_GameWindow, 1, 1, "You died!");
        nodelay(m_GameWindow, false);
        wgetch(m_GameWindow);
    }
}

int CGame::gameLoop(bool & player1Alive, bool & player2Alive) {
    clearExplosionOverlay();
    drawLevel();
    while ((m_Input = wgetch(m_GameWindow)) != ERR) {
        m_Inputs.insert(m_Input);
    }
    if (!processInput()) {
        drawEntities();
        wrefresh(m_GameWindow);
        return ATTR_NONEXISTENT;    //return state for user hitting "quit"
    }
    applyPowerUps();
    if (m_GameMode == GAMEMODE_SINGLEPLAYER)
        if (m_AIDelay-- <= 0) {
            moveNPCs();
            m_AIDelay = AI_MOVEMENT_DELAY;
        }
    explodeBombs();
    drawEntities();
    drawExplosionOverlay();
    attackPlayer();
    if (!clearDeadEntities(player1Alive, player2Alive))
        return GAME_WIN;            //return state for player winning in single player mode
    wrefresh(m_GameWindow);

    std::this_thread::sleep_for(std::chrono::milliseconds(GAME_SPEED));
    return GAME_CONTINUE;           //return state indicating for the game to keep looping
}

bool CGame::processInput() {
    for (auto it : m_Inputs) {
        switch(it) {
            case (GAME_QUIT):
                return false;
            case (PLAYER_1_UP):
                moveY(m_Entities[0], -1);
                break;
            case (PLAYER_1_DOWN):
                moveY(m_Entities[0], 1);
                break;
            case (PLAYER_1_LEFT):
                moveX(m_Entities[0], -1);
                break;
            case (PLAYER_1_RIGHT):
                moveX(m_Entities[0], 1);
                break;
            case (PLAYER_1_SETBOMB):
                placeBomb(m_Entities[0]);
                break;
            case (PLAYER_1_DETONATE):
                if (m_Entities[0]->getAttribute(ATTR_REMOTE) == 1) {
                    explodeBombs(DISPLAY_PLAYER_1);
                }
                break;
            default:
                break;
        }
        if (m_GameMode == GAMEMODE_MULTIPLAYER)
            switch(it) {
                case (PLAYER_2_UP):
                    moveY(m_Entities[1], -1);
                    break;
                case (PLAYER_2_DOWN):
                    moveY(m_Entities[1], 1);
                    break;
                case (PLAYER_2_LEFT):
                    moveX(m_Entities[1], -1);
                    break;
                case (PLAYER_2_RIGHT):
                    moveX(m_Entities[1], 1);
                    break;
                case (PLAYER_2_SETBOMB):
                    placeBomb(m_Entities[1]);
                    break;
                case (PLAYER_2_DETONATE):
                    if (m_Entities[1]->getAttribute(ATTR_REMOTE) == 1) {
                        explodeBombs(DISPLAY_PLAYER_2);
                    }
                    break; 
                default:
                    break;               
            }
    }
    m_Inputs.clear();
    return true;
}

void CGame::drawLevel() {
    wmove(m_GameWindow, 1, 1);
    for (int i = 0; i < (m_Ymax); ++i) {
        for (int j = 0; j < (m_Xmax); ++j) {
            waddch(m_GameWindow, m_Level.find(std::make_pair(j,i))->second);
        }
        wmove(m_GameWindow, i + 2, 1);
    }
}

void CGame::drawEntities() {
    for (auto it : m_Entities) {
        mvwaddch(m_GameWindow, it->getY() + 1, it->getX() + 1, it->getDisplay());
    }
}

void CGame::drawExplosionOverlay() {
    for (auto it : m_ExplosionOverlay) {
        mvwaddch(m_GameWindow, it.first.second + 1, it.first.first + 1, DISPLAY_EXPLOSION);
    }
}

void CGame::clearExplosionOverlay() {
    if (!m_ExplosionOverlay.empty())
        m_ExplosionOverlay.clear();
}

bool CGame::moveX(CGameEntity * entity, const int & amt) {
    std::pair<int,int> temp = entity->getPos();
    temp.first += amt;
    auto it = m_Level.find(temp);
    if (it != m_Level.end() && (it->second == DISPLAY_TILE_EMPTY || it->second == DISPLAY_TILE_POWERUP)) {
        entity->moveX(amt);
        return true;
    }
    return false;
}

bool CGame::moveY(CGameEntity * entity, const int & amt) {
    std::pair<int,int> temp = entity->getPos();
    temp.second += amt;
    auto it = m_Level.find(temp);
    if (it != m_Level.end() && (it->second == DISPLAY_TILE_EMPTY || it->second == DISPLAY_TILE_POWERUP)) {
        entity->moveY(amt);
        return true;
    }
    return false;
}

void CGame::moveNPCs() {
    int direction;
    bool wasMoved = false;
    for (auto it = m_Entities.begin(); it != m_Entities.end(); ++it) {
        direction = (*it)->getAttribute(ATTR_DIRECTION);
        if (direction != ATTR_NONEXISTENT) {
            while (!wasMoved) {
                switch(direction) {
                    case (DIRECTION_UP):
                        wasMoved = moveY(*it, -1);
                        break;
                    case (DIRECTION_DOWN):
                        wasMoved = moveY(*it, 1);
                        break;
                    case (DIRECTION_LEFT):
                        wasMoved = moveX(*it, -1);
                        break;
                    case (DIRECTION_RIGHT):
                        wasMoved = moveX(*it, 1);
                        break;
                    default:
                        break;
                }
                if (!wasMoved) {    //if move wasn't successful, reroll direction and try again
                    direction = std::rand() % 4;
                    (*it)->modify(ATTR_DIRECTION, direction);
                }
            }
            wasMoved = false;       //reset control value
        }
    }
}

void CGame::attackPlayer() {
    for (auto it = m_Entities.begin() + 1; it != m_Entities.end(); ++it) {
        if ((*it)->getDisplay() == DISPLAY_NPC && (*it)->getPos() == m_Entities[0]->getPos()) {
            m_Entities[0]->modify(ATTR_DAMAGE, 1);
        }
    }
}

bool CGame::clearDeadEntities(bool & player1Alive, bool & player2Alive) {
    //int temp;
    for (auto it = m_Entities.begin(); it != m_Entities.end(); ) {
        if ((*it)->getAttribute(ATTR_HP) == 0) {

            if ((*it)->getDisplay() == DISPLAY_PLAYER_1)
                player1Alive = false;
            if ((*it)->getDisplay() == DISPLAY_PLAYER_2)
                player2Alive = false;

            if (!player1Alive || !player2Alive)
                return true;

            if ((*it)->getDisplay() == DISPLAY_NPC)
                m_Entities[0]->modify(ATTR_SCORE, SCORE_KILL_BONUS);
            
            delete *it;
            it = m_Entities.erase(it);
        } else {
            ++it;
        }
    }
    if (m_GameMode == GAMEMODE_SINGLEPLAYER && m_Entities.size() == 1)
        return false; //return false if every entity except the player is dead
    return true;
}

void CGame::placeBomb(CGameEntity * player) {
    if (player->getAttribute(ATTR_BOMB_CNT) == player->getAttribute(ATTR_MAX_BOMB_CNT))
        return;
    
    CGameEntityBomb * bomb = new CGameEntityBomb(player->getX(), player->getY(), DISPLAY_BOMB, player);
    m_Entities.push_back(bomb);
    player->modify(ATTR_BOMB_CNT, 1);
}

void CGame::explodeBombs() {
    for (auto it = m_Entities.begin(); it != m_Entities.end(); ) {
        if ((*it)->getAttribute(ATTR_TIME_PLACED) == ATTR_DETONATE) {
            calculateExplosion(it);
            delete *it;
            it = m_Entities.erase(it);
        } else {
            ++it;
        }
    }
}

void CGame::explodeBombs(const char & player) {
    for (auto it = m_Entities.rbegin(); it != m_Entities.rend(); ++it) {
        if ((*it)->getAttribute(ATTR_SET_BY) == player) {
        calculateExplosion(it.base()-1);
        delete *it;
        m_Entities.erase(it.base()-1);
        break;
        }
    }
    return;
}

void CGame::calculateExplosion(std::vector<CGameEntity *>::iterator it) {
    int radius = (*it)->getAttribute(ATTR_RADIUS);
    int posX = (*it)->getX();
    int posY = (*it)->getY();
    m_ExplosionOverlay.insert(std::pair<std::pair<int,int>,char>(std::make_pair(posX, posY), DISPLAY_EXPLOSION));
        for (auto it2 : m_Entities) {
        if (it2->getPos() == std::make_pair(posX, posY))
            it2->modify(ATTR_DAMAGE, 1);
    }
    std::pair<int,int> coords;
    //have to calculate each explosion direction independently
    for (int i = 1; i <= radius; ++i) {
        if (!createExplosionOverlay(posX, posY + i))
            break;
    }
    for (int i = 1; i <= radius; ++i) {
        if (!createExplosionOverlay(posX, posY - i))
            break;
    }
    for (int i = 1; i <= radius; ++i) {
        if (!createExplosionOverlay(posX + i, posY))
            break;
    }
    for (int i = 1; i <= radius; ++i) {
        if (!createExplosionOverlay(posX - i, posY))
            break;
    }
    //subtract from current bomb count of player who placed the bomb
    if ((*it)->getAttribute(ATTR_SET_BY) == DISPLAY_PLAYER_1)
        m_Entities[0]->modify(ATTR_BOMB_CNT, -1);
    else
    if ((*it)->getAttribute(ATTR_SET_BY) == DISPLAY_PLAYER_2)
        m_Entities[1]->modify(ATTR_BOMB_CNT, -1);
}

bool CGame::createExplosionOverlay(const int & posX, const int & posY) {
    std::pair<int,int> coords;
    char tile;
    coords = std::make_pair(posX, posY);
    auto it = m_Level.find(coords);
    if (it == m_Level.end())
        return false;
    
    tile = it->second;

    if (tile == DISPLAY_TILE_UNBREAKABLE)
        return false;
    if (tile == DISPLAY_TILE_BREAKABLE) {
        m_ExplosionOverlay.insert(std::pair<std::pair<int,int>,char>(coords, DISPLAY_EXPLOSION));
        int roll = std::rand() % 100 + 1;   //roll for powerup spawn
        if (roll <= POWERUP_SPAWN_CHANCE && DISABLE_ALL_POWERUPS == 0 && m_PowerUpList.size() != 0)
            m_Level.at(coords) = DISPLAY_TILE_POWERUP;
        else
            m_Level.at(coords) = DISPLAY_TILE_EMPTY;
        return false;
    }
    if (tile == DISPLAY_TILE_EMPTY || tile == DISPLAY_TILE_POWERUP) {
        m_ExplosionOverlay.insert(std::pair<std::pair<int,int>,char>(coords, DISPLAY_EXPLOSION));
        for (auto it : m_Entities) {
            if (it->getPos() == coords)
                it->modify(ATTR_DAMAGE, 1);
        }
        return true;
    }
    return false;
}

void CGame::configurePowerups() {
    if (DISABLE_ALL_POWERUPS)
        return;
    
    if (POWERUP_HP)
        m_PowerUpList.push_back(POWERUP_HP);
    if (POWERUP_POW)
        m_PowerUpList.push_back(POWERUP_POW);
    if (POWERUP_MAX_BOMB_CNT)
        m_PowerUpList.push_back(POWERUP_MAX_BOMB_CNT);
    if (POWERUP_REMOTE)
        m_PowerUpList.push_back(POWERUP_REMOTE);
}

void CGame::calculatePowerUp(CGameEntity * player) {
    int size = m_PowerUpList.size();
    if (!size)
        return;
    int index = (std::rand() % size);
    //have to use if statements instead of switch because toggling powerups off in the config
    //is done by setting these values to zero which means switch gets duplicate conditions
    //which means this breaks!
    if (m_PowerUpList[index] == POWERUP_HP) {
            player->modify(ATTR_ONEUP, POWERUP_MAX_BOMB_CNT_STRENGTH);
            return;
    }
    if (m_PowerUpList[index] == POWERUP_POW) {
            player->modify(ATTR_POW, POWERUP_POW_STRENGTH);
            return;
    }
    if (m_PowerUpList[index] == POWERUP_MAX_BOMB_CNT) {
            player->modify(ATTR_MAX_BOMB_CNT, POWERUP_MAX_BOMB_CNT_STRENGTH);
            return;
    }
    if (m_PowerUpList[index] == POWERUP_REMOTE) {
        player->modify(ATTR_REMOTE, 1);
        //code for removing the remote powerup in case it's no longer needed
        int hasRemote1 = m_Entities[0]->getAttribute(ATTR_REMOTE);
        int hasRemote2 = m_Entities[1]->getAttribute(ATTR_REMOTE);

        if (hasRemote1 == 1 && (hasRemote2 == 1 || hasRemote2 == ATTR_NONEXISTENT))
            m_PowerUpList.erase(m_PowerUpList.begin() + index);
    }
}

void CGame::applyPowerUps() {
    if (DISABLE_ALL_POWERUPS != 0)
        return;
    
    auto coords = m_Entities[0]->getPos();
    if (m_Level.at(coords) == DISPLAY_TILE_POWERUP) {
        calculatePowerUp(m_Entities[0]);
        m_Level.at(coords) = DISPLAY_TILE_EMPTY;
    }
    
    if (m_GameMode == GAMEMODE_MULTIPLAYER) {
        coords = m_Entities[1]->getPos();
        if (m_Level.at(coords) == DISPLAY_TILE_POWERUP) {
            calculatePowerUp(m_Entities[1]);
            m_Level.at(coords) = DISPLAY_TILE_EMPTY;
        }
    }
}

int CGame::getScore() const {
    return m_Entities[0]->getAttribute(ATTR_SCORE);
}