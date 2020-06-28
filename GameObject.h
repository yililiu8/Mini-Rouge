//
//  GameObjects.hpp
//  Proj 3
//
//  Created by Yili Liu on 5/11/20.
//  Copyright © 2020 Yili Liu. All rights reserved.
//

#ifndef GameObject_h
#define GameObject_h

#include <string>
class Actor;
class Player;
class Dungeon; 
using namespace std;

class Object
{
public:
    Object(Dungeon* d, int row, int col);
    virtual string name() = 0;
    virtual void action(Actor* p) = 0;
    virtual ~Object();
    void setPos(int r, int c);
    
    //getters
    int getRow() const;
    int getCol() const;
    
private:
    Dungeon* m_d;
    int m_row;
    int m_col;
};

class Weapon
{
public:
    Weapon(string action, int bonus, int damage);
    bool hit(Actor *attacker, Actor *defender);
    int damage(Actor *attacker);
    string getAction() const;
    virtual string name() = 0;
    virtual void action(Actor* p) =0; 
    virtual ~Weapon();
private:
    string m_action;
    int m_dexBonus;
    int m_damage;
};

// *********** WEAPONS ***********
class Mace: public Object, public Weapon
{
public:
    Mace();
    void action(Actor* p);
    Mace(Dungeon* d, int row, int col);
    virtual string name();
};

class ShortSword: public Object, public Weapon
{
public:
    ShortSword();
    void action(Actor* p);
    ShortSword(Dungeon* d, int row, int col);
    virtual string name();
};

class LongSword: public Object, public Weapon
{
public:
    LongSword();
    void action(Actor* p);
    LongSword(Dungeon* d, int row, int col);
    virtual string name();
};

class MagicAxe: public Object, public Weapon
{
public:
    MagicAxe();
    MagicAxe(Dungeon* d, int row, int col);
    void action(Actor* p);
    virtual string name();
};

class MagicFangs: public Object, public Weapon
{
public:
    MagicFangs();
    MagicFangs(Dungeon* d, int row, int col);
    void action(Actor* p);
    virtual string name();
};

// ********** SCROLLS ************
class Scroll
{
public:
    virtual void action(Actor* p) = 0;
};

class Teleportation : public Object, public Scroll
{
public:
    Teleportation();
    Teleportation(Dungeon* d, int row, int col);
    void action(Actor* p);
    virtual string name();
};

class ImproveArmor : public Object, public Scroll
{
public:
    ImproveArmor();
    ImproveArmor(Dungeon* d, int row, int col);
    void action(Actor* p);
    virtual string name();
};

class RaiseStrength : public Object, public Scroll
{
public:
    RaiseStrength();
    RaiseStrength(Dungeon* d, int row, int col);
    void action(Actor* p);
    virtual string name();
};

class EnhanceHealth : public Object, public Scroll
{
public:
    EnhanceHealth();
    EnhanceHealth(Dungeon* d, int row, int col);
    void action(Actor* p);
    virtual string name();
};

class EnhanceDex : public Object, public Scroll
{
public:
    EnhanceDex(); 
    EnhanceDex(Dungeon* d, int row, int col);
    void action(Actor* p);
    virtual string name();
};

#endif /* GameObject_h */
