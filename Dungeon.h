//
//  Dungeon.hpp
//  Proj 3
//
//  Created by Yili Liu on 5/11/20.
//  Copyright © 2020 Yili Liu. All rights reserved.
//

#ifndef Dungeon_h
#define Dungeon_h

#include "Actor.h"
#include "utilities.h"
#include "GameObject.h"
#include <vector>
#include <queue>

//stores the coordinate points (for recursive goblin function)
struct Point
{
    int r;
    int c;
};
struct queueNode
{
    Point pt;  // The cordinates of a cell
    int dist;  // cell's distance of from the source
};

class Dungeon
{
public:
    Dungeon();
    ~Dungeon();
    
    //create and display board
    void emptyBoard();
    void display();
    bool valid(int rCorner, int rEnd, int cCorner, int cEnd);
    void refresh();
    void teleport(int row, int col, char &l);
    
    //add all elements to the game
    void addPlayer(int row, int col, char& l);
    void addObject(int row, int col);
    void addStairs(int row, int col);
    void addIdol(int row, int col);
    void addMonster(int row, int col);
    bool determineNewPosition(int &r, int &c, int dir, char& l);
    bool checkValid(int r, int c, int dir); //check validity of player's movements
    
    //setters
    void setLevel(int level);
    void setPos(int r, int c, char s);
    void printResult(Object* o);
    void setAction(Actor* a);
    void setResult(string s); 
    
    //object actions
    void pickUpObj(int r, int c);
    void selectWeapon(char c);
    void readScroll(char c);
    
    //monst actions
    void moveMonster(int smell);
    void playerAttack(int r, int c);
    void moveGoblin(Actor* g, int smell);
    void moveBorS(Actor* b); //Bogeyman or snake 
    bool checkValidMonster(int r, int c);
    int shortestPath(queue<queueNode> q, Point dest);
    void clearVisited();
    
    //getter functions
    int getLevel() const;
    Player* player() const;
    char getPos(int r, int c) const;
    string getResult() const;
    
private:
    Player* m_player;
    vector<Object*> m_object;
    vector<Actor*> m_monster;
    int m_level;
    char grid[18][70];
    string m_result;
    string m_action;
    int visited[18][70]; //used to keep track of positions visited in goblin function
};

#endif /* Dungeon_h */
