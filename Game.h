//
//  Game.hpp
//  Proj 3
//
//  Created by Yili Liu on 5/11/20.
//  Copyright © 2020 Yili Liu. All rights reserved.
//

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include "Dungeon.h"
// You may add data members and other member functions to this class.

class Game
{
public:
    Game(int goblinSmellDistance);
    void add(); //adds all actors and objects to the board
    void play();
    ~Game();
private:
    Dungeon* m_dungeon;
    char lastChar;
    int m_smell; 
    
};

#endif // GAME_INCLUDED
