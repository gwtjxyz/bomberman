#pragma once

#include <utility>

#include "CConstants.hpp"
/**
 * base entity class; has position and display character as attributes
 * 
 * */
class CGameEntity {
public:
    /**
     * default constructor
     */
    CGameEntity() = default;
    /**
     * default destructor
     */
    virtual ~CGameEntity() = default;
     /** *
     * @returns pos X value
     */
    int getX() const  {return m_X;}
    /** 
     * @returns pos Y value
     */
    int getY() const {return m_Y;}
    /** 
     * @returns display value
     */
    char getDisplay() const {return m_Display;}
    /**
     * abstract method for moving stuff along the X axis
     * 
     * @param[in] direction of the movement
     */
    virtual void moveX(const int & amt) = 0;
    /**
     * abstract method for moving stuff along the Y axis
     * 
     * @param[in] direction of the movement
     */
    virtual void moveY(const int & amt) = 0;
    /** abstract method;
     * modifies internal class variables based on the key, reutrns true if the modification was successful,
     * false otherwise.
     * 
     * @param[in] attribute key
     * @param[in] amount of by how much to modify
     * 
     * @returns true if modification was successful, false otherwise
     */
    virtual bool modify(const int & attrKey, const int & amt) = 0;
    /** abstract method;
     * returns an attribute based on the key passed to the function; in case of no such attribute
     * being present, returns a default error value
     * 
     * @param[in] attribute key
     * @returns the deisred parameter, if it exists
     */
    virtual int getAttribute(const int & attrKey) const = 0;
    /** function for getting the x,y of the entitiy
     *
     * @returns a pair of posx,posy of the entity
     */
    std::pair<int,int> getPos() {
        std::pair<int,int> pos = std::make_pair(m_X, m_Y);
        return pos;
    }

protected:
    int m_X;
    int m_Y;
    char m_Display;
};