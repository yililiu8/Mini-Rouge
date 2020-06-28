//
//  Actor.hpp
//  Proj 3
//
//  Created by Yili Liu on 5/11/20.
//  Copyright © 2020 Yili Liu. All rights reserved.
//

#ifndef Actor_h
#define Actor_h

#include <string>
#include <vector>

class Weapon;
#include "GameObject.h"
class Dungeon;
using namespace std;


class Actor
{
public:
    Actor(Dungeon* d, int row, int col, int hit, int armor, int strength, int dex);
    virtual string resultOfAttack(Actor* defender) = 0;
    virtual ~Actor();
    
    //setter functions
    void setPosition(int row, int col);
    void setArmor(int val);
    void setStrength(int strength);
    void setDex(int dex);
    void setSleep(int sleep);
    void setWeapon(Weapon* weapon);
    int hitMax(int bonus);
    void setHit(int hit);
    void setLast(int l);
    
    //getter functions
    Weapon* getWeapon() const;
    Dungeon* dungeon() const; 
    int getRow() const;
    int getCol() const;
    int getHit() const;
    int getArmor() const;
    int getStrength() const;
    int getDex() const;
    int getSleep() const;
    virtual string name() const = 0;
    char getLast() const;
    
private:
    int m_row; //actor's row position
    int m_col; //actor's column position
    int m_hit; //hit point (int 0 to 99)
    int m_hitMax; //maximum hit points
    Weapon* m_weapon; //weapon the action is weilding
    Dungeon* m_d;
    int m_armor; //armor points (int 0 to 99)
    int m_strength; //strength points (0 to 99)
    int m_dexterity; //dexterity points (0 to 99)
    int m_sleep; //sleep time (int 0 to 9)
    char m_last; //last char that was placed in position of the actor
};


class Player: public Actor
{
public:
    Player(Dungeon* d, int r, int c);
    virtual ~Player(); 
    //the result of picking up an object
    string resultOfObject(Object* object);
    //the result of reading a scroll
    string resultOfScroll(Object* scroll);
    //virtual int resultOfAttack(Actor* defender);
    virtual string name() const;
    //add object to the inventory
    bool addInventory(Object* object);
    //print out player inventory
    void printInventory() const;
    //set weapon based on inventoryth
    string setWeapon(char c);
    //read what is on the scroll then self destruct
    string readScroll(char c);
    //the result of an attack on a monster
    string resultOfAttack(Actor* defender);
private:
    vector<Object*> m_inventory;
};

class Bogeyman : public Actor
{
public:
    Bogeyman(Dungeon* d, int row, int col);
    virtual string name() const;
    string resultOfAttack(Actor* defender);
};


class Snakewoman : public Actor
{
public:
    Snakewoman(Dungeon* d, int row, int col);
    virtual string name() const;
    string resultOfAttack(Actor* defender);
};


class Dragon : public Actor
{
public:
    Dragon(Dungeon* d, int row, int col);
    virtual string name() const;
    string resultOfAttack(Actor* defender);
};


class Goblin: public Actor
{
public:
    Goblin(Dungeon* d, int row, int col);
    virtual string name() const;
    string resultOfAttack(Actor* defender);
};

#endif /* Actor_h */
