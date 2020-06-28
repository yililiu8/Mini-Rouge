//
//  Game.cpp
//  Proj 3
//
//  Created by Yili Liu on 5/11/20.
//  Copyright © 2020 Yili Liu. All rights reserved.
//

#include "Game.h"
#include "Dungeon.h"
#include "utilities.h"
#include <iostream>
using namespace std;

Game::Game(int goblinSmellDistance)
{
    //create dungeon
    m_dungeon = new Dungeon;
    
    //inialize last position
    lastChar = ' ';
    
    add();
    m_smell = goblinSmellDistance;
    
}

//add all of the actors and objects to the dungeon
void Game::add()
{
    m_dungeon->refresh();
    
    //add monsters
    int nMonsters = randInt(2, 5*(m_dungeon->getLevel() + 1) + 1);
    while(nMonsters != 0)
    {
        int rMonster = randInt(1, 17);
        int cMonster = randInt(1, 69);
        m_dungeon->addMonster(rMonster, cMonster);
        nMonsters--; 
    }
    
    
    //add random objects
    int nObjects = randInt(2, 3);
    while(nObjects != 0)
    {
        int rObject = randInt(1, 17);
        int cObject = randInt(1, 69);
        m_dungeon->addObject(rObject, cObject);
        nObjects--;
        
    }
    
    //add stairs
    if(m_dungeon->getLevel() < 4)
    {
        int rStairs = randInt(1, 17);
        int cStairs = randInt(1, 69);
        m_dungeon->addStairs(rStairs, cStairs);
    }
    
    //add golden idol
    if(m_dungeon->getLevel() == 4)
    {
        int rStairs = randInt(1, 17);
        int cStairs = randInt(1, 69);
        m_dungeon->addIdol(rStairs, cStairs);
    }
    
    //add player
    int rPlayer = randInt(1, 17);
    int cPlayer = randInt(1, 69);
    if(m_dungeon->getLevel() == 0)
    {
        m_dungeon->addPlayer(rPlayer, cPlayer, lastChar);
    }
    else
    {
        //teleport the player if the level is not 0
        m_dungeon->teleport(rPlayer, cPlayer, lastChar);
    }

}

Game::~Game()
{
    delete m_dungeon;
}

void Game::play()
{
    char weapon = ' ';
    char scroll = ' ';
    cout << "Press q to exit game." <<endl;
    do
    {
        //move monster
       m_dungeon->moveMonster(m_smell);
        
        //if player dies
        if(m_dungeon->player()->getHit() <= 0)
        {
            clearScreen();
            m_dungeon->display();
            exit(1);
        }
        
        //display the dungeon
        clearScreen();
        m_dungeon->display();
        
        char action = getCharacter();
        int r = m_dungeon->player()->getRow();
        int c = m_dungeon->player()->getCol();
        
        switch (action)
        {
            case 'c': //cheat sets hit to 50 and strength to 9
                m_dungeon->player()->setHit(50);
                m_dungeon->player()->setStrength(9);
                break;
            case '>': //brings player to the next level
                if(lastChar == '>')
                {
                    m_dungeon->setLevel(m_dungeon->getLevel() + 1);
                    lastChar = ' ';
                    add(); 
                }
                break;
            case 'i': //shows inventory
                clearScreen();
                m_dungeon->player()->printInventory();
                getCharacter();
                break;
            case 'w': //weild a weapon from the inventory
                clearScreen();
                m_dungeon->player()->printInventory();
                weapon = getCharacter();
                m_dungeon->selectWeapon(weapon);
                break;
            case 'r': //read a scroll from the inventory
                clearScreen();
                m_dungeon->player()->printInventory();
                scroll = getCharacter();
                m_dungeon->readScroll(scroll);
                break;
            case 'g':  //pick up an item
                if(lastChar == ')' || lastChar == '?')
                {
                    m_dungeon->pickUpObj(r, c);
                    lastChar = ' ';
                }
                if(lastChar == '&')
                {
                    m_dungeon->setResult("You pick up the golden idol.");
                    clearScreen();
                    m_dungeon->display();
                    cout << endl; 
                    cout << "Congratulations, you won!" << endl;
                    exit(1);
                }
                break; 
            case ARROW_LEFT:   //move the player
            case ARROW_RIGHT:
            case ARROW_UP:
            case ARROW_DOWN:
                //check it is a valid position
                if ( m_dungeon->checkValid(r, c, action) == false)
                    break;
                //if sleeping, dont move the player
                else if( m_dungeon->player()->getSleep() > 0)
                {
                    m_dungeon->player()->setSleep(m_dungeon->player()->getSleep() -1);
                    break;
                }
                else
                {
                    m_dungeon->determineNewPosition(r, c, action, lastChar);
                    break;
                }
            case 'q':
                exit(1);
                break;
        }
        

    } while(m_dungeon->player()->getHit() >= 0);
}

