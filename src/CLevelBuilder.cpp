#include "CLevelBuilder.hpp"

bool CLevelBuilder::buildLevel(const std::string & levelName) {
    //reset values from possible previously run level builder
    if (!m_Level.empty())
        m_Level.clear();
    if (!m_PlayerSpawnpoints.empty())
        m_PlayerSpawnpoints.clear();
    if (!m_NPCSpawnpoints.empty())
        m_NPCSpawnpoints.clear();
    if (!m_LevelName.empty())
        m_LevelName.clear();


    m_Xmax = 0;
    m_Ymax = 0;
    m_HighScore = 0;
    
    m_LevelName = levelName;
    std::fstream fs;
    fs.open(levelName, std::fstream::in);
    if (!fs.is_open())
        return false;
    

    int posX = 0, posY = 0;
    char c = 0;
    //check if file is of the correct format
    fs.get(c);
    if (c != DISPLAY_TILE_UNBREAKABLE)
        return closeFileStreamAndReturnFalse(fs);

    while (c == DISPLAY_TILE_UNBREAKABLE) { // finding out width
        m_Level.insert(std::pair<std::pair<int,int>,char>(std::make_pair(posX, posY), c));
        posX++;
        fs.get(c);
    }
    if (c != '\n')
        return closeFileStreamAndReturnFalse(fs);
    
    m_Ymax++;
    posY++;
    m_Xmax = posX;

    bool endOfLevel = false; //if we go a full row without encountering anything but #,
                             //set this to true and exit reading
    //std::cout << "while loop initiated\n";
    while (fs.get(c)) {
        if (!fs.good())
            return closeFileStreamAndReturnFalse(fs);
        
        endOfLevel = true;
        for (posX = 0; posX < m_Xmax; ++posX) { //for loop for filling a row
            if (!fs.good())
                return closeFileStreamAndReturnFalse(fs);

            if (posX == 0 && c != DISPLAY_TILE_UNBREAKABLE) //checking for start of line validity
                return closeFileStreamAndReturnFalse(fs);

            if (posX + 1 == m_Xmax && c != DISPLAY_TILE_UNBREAKABLE) //checking for end of line validity
                return closeFileStreamAndReturnFalse(fs);

            switch(c) {
                case(DISPLAY_TILE_UNBREAKABLE): //unbreakable wall
                    m_Level.insert(std::pair<std::pair<int,int>,char>(std::make_pair(posX, posY), c));
                    break;
                case(DISPLAY_TILE_EMPTY): //empty space
                    m_Level.insert(std::pair<std::pair<int,int>,char>(std::make_pair(posX, posY), c));
                    endOfLevel = false;
                    break;
                case(DISPLAY_TILE_BREAKABLE): //breakable wall
                    m_Level.insert(std::pair<std::pair<int,int>,char>(std::make_pair(posX, posY), c));
                    endOfLevel = false;
                    break;
                case(DISPLAY_NPC_SPAWN): //NPC spawnpoint
                    m_Level.insert(std::pair<std::pair<int,int>,char>(std::make_pair(posX, posY), DISPLAY_TILE_EMPTY));
                    endOfLevel = false;
                    m_NPCSpawnpoints.push_back(std::make_pair(posX, posY));
                    break;
                case(DISPLAY_PLAYER_SPAWN): //player spawnpoint
                    m_Level.insert(std::pair<std::pair<int,int>,char>(std::make_pair(posX, posY), DISPLAY_TILE_EMPTY));
                    endOfLevel = false;  
                    m_PlayerSpawnpoints.push_back(std::make_pair(posX, posY));
                    break;
                default:
                    return closeFileStreamAndReturnFalse(fs);
            }
            fs.get(c);
        }
        if (c != '\n')
            return closeFileStreamAndReturnFalse(fs);
        posY++;
        m_Ymax++;

        if (endOfLevel)
            break;

    }

    if (m_Xmax < 9 || m_Ymax < 7)
        return closeFileStreamAndReturnFalse(fs);
        //level is too small

    if (m_NPCSpawnpoints.size() == 0 || m_PlayerSpawnpoints.size() <= 1)
        return closeFileStreamAndReturnFalse(fs);
        //how u gonna play iwthout spawnpoints HUH?

    fs >> m_HighScore;

    if (!fs.good() && !fs.eof())
        return closeFileStreamAndReturnFalse(fs);
    
    fs.close();
    return true;
}

void CLevelBuilder::setHighScore(const int & highScore) {
    if (highScore < 0 || m_HighScore >= highScore)
        return;
    else
        m_HighScore = highScore;
    std::fstream fs;
    fs.open(m_LevelName, std::fstream::in | std::fstream::out);
        if (!fs.is_open())
            return;

    char c;

    while (fs.get(c)) {
        if (c >= '0' && c <= '9')
            break;
    }
    fs.unget();

    std::string scoreString = std::to_string(highScore);
    fs << scoreString;
}

void CLevelBuilder::getLevelDimensions(int & x, int & y) {
    x = m_Xmax;
    y = m_Ymax;
}

bool CLevelBuilder::closeFileStreamAndReturnFalse(std::fstream & fs) {
    fs.close();
    return false;
}