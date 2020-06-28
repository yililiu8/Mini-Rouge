//
//  Actor.cpp
//  Proj 3
//
//  Created by Yili Liu on 5/11/20.
//  Copyright © 2020 Yili Liu. All rights reserved.
//

#include "Actor.h"
#include "Dungeon.h"
#include "GameObject.h"
#include <iostream>
#include <vector>

// ******* Actor class ********

//implememnt weapon later
Actor::Actor(Dungeon* d, int row, int col, int hit, int armor, int strength, int dex)
{
    m_d = d;
    m_weapon = new ShortSword;
    m_hit = hit;
    m_armor = armor;
    m_strength = strength;
    m_dexterity = dex;
    m_sleep = 0;
    m_row = row;
    m_col = col;
    m_hitMax = 100;
    m_last = ' ';
    
}

Actor::~Actor()
{
    delete m_weapon;
}

Dungeon* Actor::dungeon() const
{
    return m_d;
}

char Actor::getLast() const
{
    return m_last;
}

void Actor::setLast(int l)
{
    m_last = l; 
}

//sets weapon
void Actor::setWeapon(Weapon* weapon)
{
    m_weapon = weapon;
}

//returns the the weapon being used
Weapon* Actor::getWeapon() const
{
    return m_weapon;
}

//returns the bonus on hit max
int Actor::hitMax(int bonus)
{
    return m_hitMax + bonus;
}

//sets the hit value
void Actor::setHit(int hit)
{
    if(hit < m_hitMax)
        m_hit = hit;
}

//sets position of the actor on the board
void Actor::setPosition(int row, int col)
{
    m_row = row;
    m_col = col;
}

//sets armor value
void Actor::setArmor(int val)
{
    if(val >= 0 && val < 100 )
        m_armor = val;
}

//set strength points value
void Actor::setStrength(int strength)
{
    if(strength >= 0 && strength < 100)
        m_strength = strength;
}

//set dexerity points
void Actor::setDex(int dex)
{
    if(dex >= 0 && dex < 100)
        m_dexterity = dex;
}

//set sleep time value
void Actor::setSleep(int sleep)
{
    if(sleep >= 0 && sleep < 10)
        m_sleep = sleep;
}

//get row value
int Actor::getRow() const
{
    return m_row;
}

//get column value
int Actor::getCol() const
{
    return m_col;
}

//get hit points value
int Actor::getHit() const
{
    return m_hit;
}

//get armor points value
int Actor::getArmor() const
{
    return m_armor;
}

//get strength points value
int Actor::getStrength() const
{
    return m_strength;
}

//get dexterity points value
int Actor::getDex() const
{
    return m_dexterity;
}

//get sleep time value
int Actor::getSleep() const
{
    return m_sleep;
}


// ******* Player class *********

Player::Player(Dungeon* d, int r, int c) : Actor(d, r, c, 20, 2, 2, 2)
{
    m_inventory.push_back(new ShortSword); 
}

Player::~Player()
{
    for (vector<Object*>::iterator p = m_inventory.begin(); p != m_inventory.end(); p++)
        delete *p;
}

string Player::name() const
{
    return "Player";
}

//return a string containing the result of picking up an object
string Player::resultOfObject(Object* object)
{
    string result = "You pick up a " + object->name() + ".";
    if(object->name() == "magic fangs")
        result = "You pick up " + object->name() + ".";
    if(object->name() =="scroll of teleportation" || object->name() == "scroll of strength" || object->name() == "scroll of enhance health" || object->name() == "scroll of enhance dexterity" || object->name() == "scroll of enhance armor")
        result = "You pick up a scroll called " + object->name() + ".";
    return result;
}

//set weapon based off of inventory letter
string Player::setWeapon(char c)
{
    string result = "";
    int n = c - 'a';
    if (n > m_inventory.size())
        result =  "Error. Not a valid inventory member.";
    //if it is not a weapon, return so and do nothing
    else if(m_inventory[n]->name() == "scroll of teleportation" || m_inventory[n]->name() == "scroll of strength" || m_inventory[n]->name() == "scroll of enhance health" || m_inventory[n]->name() == "scroll of enhance dexterity" || m_inventory[n]->name() == "scroll of enhance armor")
        result =  "You can't wield a " + m_inventory[n]->name();
    else
    {
        //add the new weapon as the current weapon and return the result of that weapon
        if(m_inventory[n]->name() == "mace")
                Actor::setWeapon(new Mace);
            else if(m_inventory[n]->name() == "short sword")
                Actor::setWeapon(new ShortSword);
            else if(m_inventory[n]->name() == "long sword")
                Actor::setWeapon(new LongSword);
            else if(m_inventory[n]->name() == "magic fangs")
                Actor::setWeapon(new MagicFangs);
            else if(m_inventory[n]->name() == "magic axe")
            Actor::setWeapon(new MagicAxe);
        result = "You are wielding a " + m_inventory[n]->name();
    }
    return result;
}

//read a scroll and return a string containing it's result
string Player::readScroll(char c)
{
    string result = "";
    int n = c - 'a';
    if (n > m_inventory.size())
        result = "Error. Not a valid inventory member.";
    //make sure it is a valid scroll
    else if(m_inventory[n]->name() == "mace" || m_inventory[n]->name() == "short sword" || m_inventory[n]->name() == "long sword" || m_inventory[n]->name() == "magic fangs" || m_inventory[n]->name() == "magic axe")
        result = "You can't read a " + m_inventory[n]->name();
    else
    {
        //read scroll and erase it from your inventory
        m_inventory[n]->action(this);
        result = resultOfScroll(m_inventory[n]);
        m_inventory.erase(m_inventory.begin() + n);
    }
    return result;
}


bool Player::addInventory(Object* object)
{
    // if the inventory is full dont add the item
    if(m_inventory.size() > 25)
        return false;
    
    //add the object to your inventory based off of it's name
    if(object->name() == "mace")
        m_inventory.push_back(new Mace);
    else if (object->name() == "short sword")
        m_inventory.push_back(new ShortSword);
    else if (object->name() == "long sword")
        m_inventory.push_back(new LongSword);
    else if (object->name() == "magic axe")
        m_inventory.push_back(new MagicAxe);
    else if (object->name() == "magic fangs")
        m_inventory.push_back(new MagicFangs);
    else if (object->name() == "scroll of teleportation")
        m_inventory.push_back(new Teleportation);
    else if (object->name() == "scroll of enhance armor")
        m_inventory.push_back(new ImproveArmor);
    else if (object->name() == "scroll of strength")
        m_inventory.push_back(new RaiseStrength);
    else if (object->name() == "scroll of enhance health")
        m_inventory.push_back(new EnhanceHealth);
    else if (object->name() == "scroll of enhance dexterity")
        m_inventory.push_back(new EnhanceDex);
    return true;
    
}

//print the inventory
void Player::printInventory() const
{
    char c = ' ';
    cout << "Inventory:" << endl;
    for(int k = 0; k < m_inventory.size(); k++)
    {
        c = 'a' + k;
        cout << " " << c << ". " << m_inventory[k]->name() << endl;
    }
}

//display the result of a scroll based on it's name
string Player::resultOfScroll(Object* scroll)
{
    string result = "";
    if(scroll->name() == "scroll of teleportation")
        result = "You read a " + scroll->name() + ". You feel your body wrenched in space and time.";
    else if(scroll->name() == "scroll of strength")
        result = "You read a " + scroll->name() + ". Your muscles bulge.";
    else if(scroll->name() == "scroll of enhance armor")
        result = "You read a " + scroll->name() + ". Your armor glows blue.";
    else if(scroll->name() == "scroll of enhance health")
        result = "You read a " + scroll->name() + ". You feel your heart beating stronger.";
    else if (scroll->name() == "scroll of enhance dexterity")
        result = "You read a " + scroll->name() + ". You feel like less of a klutz.";
    return result; 
}

//The result of an attack of one actor on another must be displayed.
string Player::resultOfAttack(Actor* defender)
{
    string message = "";
    //determine whether the monster hits or misses
    bool hit = getWeapon()->hit(this, defender);
    string result = "";
    if(hit == true)
        result = "hits";
    else if(hit == false)
        result = "misses";

    //do damage to the defender
    if(hit == true)
    {
        defender->setHit(defender->getHit() - getWeapon()->damage(this));
        if(getWeapon()->name() == "magic fangs") //only applies towards magic fangs
        {
            getWeapon()->action(defender);
            if(defender->getSleep() > 0)
            {
                message = "Player " + getWeapon()->getAction() + " " + getWeapon()->name() + " at the " +  defender->name() +" and " + result + ", putting " + defender->name() + " to sleep.";
                return message;
            }
        }
    }
    
    //return the message
    message = "Player " + getWeapon()->getAction() + " " + getWeapon()->name() + " at the " +  defender->name() +" and " + result;
    if(defender->getHit() <= 0)
        message += " dealing a final blow";
    message += ".";
    return message;
}


// ************ Bogeyman Class **************

Bogeyman::Bogeyman(Dungeon* d, int row, int col) : Actor(d, row, col, randInt(5, 10), 2, randInt(2, 3), randInt(2, 3))
{
    
}

string Bogeyman::name() const
{
    return "Bogeyman";
}

string Bogeyman::resultOfAttack(Actor* defender)
{
    string message = "";
    //determine whether the monster hits or misses
    bool hit = getWeapon()->hit(this, defender);
    string result = "";
    if(hit == true)
        result = "hits";
    else if(hit == false)
        result = "misses";

    //do damage to the defender
    if(hit == true)
    {
        defender->setHit(defender->getHit() - getWeapon()->damage(this));
    }
    
    //return the message
    message = "Bogeyman " + getWeapon()->getAction() + " " + getWeapon()->name() + " at the Player and " + result;
    if(defender->getHit() <= 0)
        message += " dealing a final blow";
    message += ".";
    
    return message;
}

// ************ Snakewoman Class ***************

Snakewoman::Snakewoman(Dungeon* d, int row, int col) : Actor(d, row, col, randInt(3, 6), 3, 2, 3)
{
    setWeapon(new MagicFangs);
}

string Snakewoman::name() const
{
    return "Snakewoman";
}

string Snakewoman::resultOfAttack(Actor* defender)
{
    string message = "";
    //determine whether the monster hits or misses
    bool hit = getWeapon()->hit(this, defender);
    string result = "";
    if(hit == true)
        result = "hits";
    else if(hit == false)
        result = "misses";

    //do damage to the defender
    if(hit == true)
    {
        defender->setHit(defender->getHit() - getWeapon()->damage(this));
        getWeapon()->action(defender);
    }
    
    //return the message
    message = "Snakewoman " + getWeapon()->getAction() + " " + getWeapon()->name() + " at the Player and " + result;
    if(defender->getHit() <= 0)
        message += " dealing a final blow";
    else if(defender->getSleep() > 0)
    {
        message += ", putting " + defender->name() + " to sleep";
    }
    message += ".";
    return message;
}


// ************ Dragon Class **************

Dragon::Dragon(Dungeon* d, int row, int col) : Actor(d, row, col, randInt(20, 25), 4, 4, 4)
{
    setWeapon(new LongSword);
}

string Dragon::name() const
{
    return "Dragon";
}


string Dragon::resultOfAttack(Actor* defender)
{
    string message = "";
    //determine whether the monster hits or misses
    bool hit = getWeapon()->hit(this, defender);
    string result = "";
    if(hit == true)
        result = "hits";
    else if (hit == false)
        result = "misses";

    //do damage to the defender
    if(hit == true)
    {
        int damage = getWeapon()->damage(this);
        if(damage < 0)
            damage = damage * -1;
        defender->setHit(defender->getHit() - damage);
    }
    
    //return the message
    message = "Dragon " + getWeapon()->getAction() + " " + getWeapon()->name() + " at the Player and " + result;
    if(defender->getHit() <= 0)
        message += " dealing a final blow.";
    message += ".";
    return message;
}


// ************ Goblin Class ***************

Goblin::Goblin(Dungeon* d, int row, int col) : Actor(d, row, col, randInt(15, 20), 1, 3, 1)
{
    
}

string Goblin::name() const
{
    return "Goblin"; 
}

string Goblin::resultOfAttack(Actor* defender)
{
    string message = "";
    //determine whether the monster hits or misses
    bool hit = getWeapon()->hit(this, defender);
    string result = "";
    if(hit == true)
        result = "hits";
    else if(hit == false)
        result = "misses";

    //do damage to the defender
    if(hit == true)
    {
        defender->setHit(defender->getHit() - getWeapon()->damage(this));
    }
    
    //return the message
    message = "Goblin " + getWeapon()->getAction() + " " + getWeapon()->name() + " at the Player and " + result;
    if(defender->getHit() <= 0)
        message += " dealing a final blow";
    message += ".";
    return message;
}
