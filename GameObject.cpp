//
//  GameObjects.cpp
//  Proj 3
//
//  Created by Yili Liu on 5/11/20.
//  Copyright © 2020 Yili Liu. All rights reserved.
//

#include "GameObject.h"
#include "utilities.h"
#include "Actor.h"
// ******** Object Class *********

Object::Object(Dungeon*d, int row, int col)
{
    m_d = d;
    m_row = row;
    m_col = col;
}

Object::~Object()
{
    
}

//get row value
int Object::getRow() const
{
    return m_row;
}

//get col value
int Object::getCol() const
{
    return m_col;
}

//change the position of the object if needed
void Object::setPos(int r, int c)
{
    m_row = r;
    m_col = c; 
}

// ******** Weapon Class *********

Weapon::Weapon(string action, int bonus, int damage)
{
    m_action = action;
    m_dexBonus = bonus;
    m_damage = damage;
}

string Weapon::getAction() const
{
    return m_action;
}

Weapon::~Weapon()
{
    
}

//determines whether an attacker hits the defender
bool Weapon::hit(Actor *attacker, Actor *defender)
{
    int attackerPoints = attacker->getDex() + m_dexBonus;
    int defenderPoints = defender->getDex() + defender->getArmor();
    if (randInt(1, attackerPoints) >= randInt(1, defenderPoints))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//determines the weapon's damange amount to an actor
int Weapon::damage(Actor* attacker)
{
    int damagePoints = randInt(0, attacker->getStrength() + m_damage - 1);
    return damagePoints;
}


// ******* Mace Class ********

Mace::Mace(): Object(nullptr, 0, 0), Weapon("swings", 0, 2)
{
    
}

//no extra action needed
void Mace::action(Actor* p)
{
    p = p;
}

Mace::Mace(Dungeon* d, int row, int col) : Object(d, row, col), Weapon("swings", 0, 2)
{
    
}

string Mace::name()
{
    return "mace";
}

// ******* ShortSword Class ********

ShortSword::ShortSword(): Object(nullptr, 0, 0), Weapon("slashes", 0, 2)
{
    
}

ShortSword::ShortSword(Dungeon* d, int row, int col) : Object(d, row, col), Weapon( "slashes", 0, 2)
{
    
}

//no extra action needed
void ShortSword::action(Actor* p)
{
    p = p;
}

string ShortSword::name()
{
    return "short sword";
}

// ******* LongSword Class ********

LongSword::LongSword() : Object(nullptr, 0, 0), Weapon("swings", 2, 4)
{
    
}

LongSword::LongSword(Dungeon* d, int row, int col): Object(d, row, col), Weapon( "swings", 2, 4)
{
    
}

//no extra action needed
void LongSword::action(Actor* p)
{
    p = p; 
}

string LongSword::name()
{
    return "long sword";
}

// ******* MagicAxe Class ********

MagicAxe::MagicAxe() : Object(nullptr, 0, 0), Weapon("chops", 5, 5)
{
    
}

MagicAxe::MagicAxe(Dungeon* d, int row, int col): Object(d, row, col), Weapon("chops", 5, 5)
{
    
}

//no extra action needed
void MagicAxe::action(Actor* p)
{
    p = p;
}

string MagicAxe::name()
{
    return "magic axe";
}


// ******* MagicFangs Class ********

MagicFangs::MagicFangs() : Object(nullptr, 0, 0), Weapon("strikes", 3, 2)
{
    
}


MagicFangs::MagicFangs(Dungeon* d, int row, int col): Object(d, row, col), Weapon( "strikes", 3, 2)
{
    
}

//puts player to sleep is probability is true
void MagicFangs::action(Actor* p)
{
    if(trueWithProbability(.2) == true)
    {
        int sleep = randInt(2, 6);
        if(p->getSleep() > sleep)
            p->setSleep(p->getSleep());
        else
            p->setSleep(sleep);
    }
}

string MagicFangs::name()
{
    return "magic fangs";
}


// ********* Teleportation Class ***********

Teleportation::Teleportation() : Object(nullptr, 0, 0)
{
    
}

Teleportation::Teleportation(Dungeon* d, int row, int col) : Object(d, row, col)
{
    
}

//The player is randomly moved to another place in the level that is not occupied by a wall or a monster.
void Teleportation::action(Actor* p)
{
    p->setPosition(randInt(1, 17), randInt(1, 69));
}

string Teleportation::name()
{
    return "scroll of teleportation";
}


// ******** ImproveArmor Class ***********

ImproveArmor::ImproveArmor() : Object(nullptr, 0, 0)
{
    
}

ImproveArmor::ImproveArmor(Dungeon* d, int row, int col) : Object(d, row, col)
{
}

//sets armor to a random int from 1 to 3
void ImproveArmor::action(Actor* p)
{
    p->setArmor(p->getArmor() + randInt(1, 3));
}

string ImproveArmor::name()
{
    return "scroll of enhance armor";
}


// ******** RaiseStrength Class **********

RaiseStrength::RaiseStrength() : Object(nullptr, 0, 0)
{
    
}

RaiseStrength::RaiseStrength(Dungeon* d, int row, int col) : Object(d, row, col)
{
    
}

//player's strength points are increased by a random int from 1 to 3
void RaiseStrength::action(Actor* p)
{
    p->setStrength(p->getStrength() + randInt(1, 3));
}

string RaiseStrength::name()
{
    return "scroll of strength";
}


// ********** EnhanceHealth Class ***********

EnhanceHealth::EnhanceHealth() : Object(nullptr, 0, 0)
{
    
}

EnhanceHealth::EnhanceHealth(Dungeon* d, int row, int col) : Object(d, row, col)
{
    
}

 //player's maximum hit point value is increased by a random int from 3 to 8. This scroll does not affect the player's current number of hit points
void EnhanceHealth::action(Actor *p)
{
    p->hitMax(randInt(3,8)); 
}

string EnhanceHealth::name()
{
    return "scroll of enhance health";
}


// *********** EnhanceDex Class *************

EnhanceDex::EnhanceDex() : Object(nullptr, 0, 0)
{
    
}

EnhanceDex::EnhanceDex(Dungeon* d, int row, int col) : Object(d, row, col)
{
}

//sets dexterity to be one more
void EnhanceDex::action(Actor* p)
{
    p->setDex(p->getDex() + 1);
}

string EnhanceDex::name()
{
    return "scroll of enhance dexterity";
}

