//
//  Dungeon.cpp
//  Proj 3
//
//  Created by Yili Liu on 5/11/20.
//  Copyright © 2020 Yili Liu. All rights reserved.
//

#include "Dungeon.h"
#include "utilities.h"
#include <iostream>
#include <string>
#include <vector>
#include <queue>
using namespace std;

// ************* Dungeon Class *************

Dungeon::Dungeon()
{
    m_level = 0; 
    m_player = nullptr;
    emptyBoard();
    m_result = "";
    m_action = "";
    clearVisited();
}

Dungeon::~Dungeon()
{
    delete m_player;
    for (vector<Object*>::iterator p = m_object.begin(); p != m_object.end(); p++)
        delete *p;
    for (vector<Actor*>::iterator p = m_monster.begin(); p != m_monster.end(); p++)
        delete *p;
}

//refreshes the board for a new level
void Dungeon::refresh()
{
    for(int k = 0; k < m_object.size(); k++)
    {
        m_object.erase(m_object.begin() + k);
    }
    for(int k = 0; k < m_monster.size(); k++)
    {
        m_monster.erase(m_monster.begin() + k);
    }
     
    emptyBoard();
}

//clears visited, an array used to keep track of position in goblin function
void Dungeon::clearVisited()
{
    for(int k =0 ; k < 18; k++)
        for(int j = 0; j < 70; j++)
            visited[k][j] = 0;
}

//move goblin through breadth-first search
void Dungeon::moveGoblin(Actor* g, int smell)
{
    //set max steps to a large number
     int min_up = 600;
     int min_down = 600;
     int min_left = 600;
     int min_right = 600;
    //check that the player is in the radius
    for(int r = g->getRow() -smell; r < g->getRow() + smell; r++)
    {
        for(int c = g->getCol() - smell; c < g->getCol() + smell; c++)
        {
            if(r >= 1 && c >= 1 && r <= 17 && c <= 68)
            {
            if(grid[r][c] == '@')
            {
                //create player point
                Point p = {m_player->getRow(), m_player->getCol()};
                
                //up
                Point up = {g->getRow()-1, g->getCol()}; //up direction
                visited[up.r][up.c] = 1; //set that spot to visited
                queue<queueNode> q; //push that into the node
                queueNode s = {up, 0};
                q.push(s);
                
                //down
                 Point down = {g->getRow()+1, g->getCol()};
                visited[down.r][down.c] = 1;
                queue<queueNode> d;
                queueNode d1 = {down, 0};
                d.push(d1);
                
                //left
                 Point left = {g->getRow(), g->getCol()-1};
                visited[left.r][left.c] = 1;
                queue<queueNode> l;
                queueNode l1 = {left, 0};
                l.push(l1);
                
                //right
                 Point right = {g->getRow(), g->getCol()+1};
                visited[right.r][right.c] = 1;
                queue<queueNode> r;
                queueNode r1 = {right, 0};
                r.push(r1);
                
                
                //to reduce the amount of times the helper function is run
                if(m_player->getRow() >= g->getRow() && m_player->getCol() <= g->getCol())
                {
                    //uses goblin recursive function
                    min_down = shortestPath(d, p);
                    clearVisited(); //clears visited for another run
                    min_left = shortestPath(l, p);
                }
                else if(m_player->getRow() >= g->getRow() && m_player->getCol() >= g->getCol())
                {
                    min_down = shortestPath(d, p);
                    clearVisited();
                    min_right = shortestPath(r , p);
                }
                else if(m_player->getRow() <= g->getRow() && m_player->getCol() >= g->getCol())
                {
                    min_up = shortestPath(q, p);
                    clearVisited();
                    min_right = shortestPath(r , p);
                }
                else if(m_player->getRow() <= g->getRow() && m_player->getCol() <= g->getCol())
                {
                    min_up = shortestPath(q, p);
                    clearVisited();
                    min_left = shortestPath(l, p);
                }
                
                
                //assign new position after running through shortestPath
                if(min_up <= min_down && min_up <= min_left && min_up <= min_right && checkValidMonster(g->getRow()-1, g->getCol()) && min_up <= smell) //up
                {
                    grid[g->getRow()][g->getCol()] = g->getLast();
                    g->setLast(grid[g->getRow()-1][g->getCol()]);
                    g->setPosition(g->getRow()-1, g->getCol());
                    grid[g->getRow()][g->getCol()] = 'G';
                }
                else if(min_down <= min_up && min_down <= min_left && min_down <= min_right && checkValidMonster(g->getRow()+1, g->getCol()) && min_down <= smell) //down
                {
                    grid[g->getRow()][g->getCol()] = g->getLast();
                    g->setLast(grid[g->getRow()+1][g->getCol()]);
                    g->setPosition(g->getRow()+1, g->getCol());
                    grid[g->getRow()][g->getCol()] = 'G';
                }
                else if(min_left <= min_down && min_left <= min_up && min_left <= min_right && checkValidMonster(g->getRow(), g->getCol()-1) && min_left <= smell) //left
                {
                    grid[g->getRow()][g->getCol()] = g->getLast();
                    g->setLast(grid[g->getRow()][g->getCol()-1]);
                    g->setPosition(g->getRow(), g->getCol()-1);
                    grid[g->getRow()][g->getCol()] = 'G';
                }
                else if(min_right <= min_down && min_right <= min_left && min_right <= min_up && checkValidMonster(g->getRow(), g->getCol()+1) && min_right <= smell ) //right
                {
                    grid[g->getRow()][g->getCol()] = g->getLast();
                    g->setLast(grid[g->getRow()][g->getCol()+1]);
                    g->setPosition(g->getRow(), g->getCol()+1);
                    grid[g->getRow()][g->getCol()] = 'G';
                }
            }
            }
        }
    }
}

//RECURSIVE GOBLIN HELPER FUNCTION!!!
int Dungeon::shortestPath(queue<queueNode> q, Point dest)
{
    //if queue becomes empty, return a large number as the distance
    if(q.empty() == true)
        return 500;
    
    //set front node to current examining position
    queueNode curr = q.front();
    Point pt = curr.pt;
    
    //if found the location, return the distance
    if (pt.r == dest.r && pt.c == dest.c)
        return curr.dist;
    
    //pop front node and set it's values to be the current row and col
    q.pop();
    int row = pt.r;
    int col = pt.c;
    
    if(row > 0 && row < 18 && col > 0 && col < 70)
    {
    //check up
    if (grid[row-1][col] != '#' && visited[row-1][col] == 0)
    {
        visited[row-1][col] = 1;
        queueNode Adjcell = { {row-1, col}, curr.dist + 1 };
        q.push(Adjcell);
    }
    //check down
    if (grid[row+1][col] != '#' && visited[row+1][col] == 0)
    {
        visited[row+1][col] = 1; //show the point is visited already
        queueNode Adjcell = { {row+1, col}, curr.dist + 1 };
        q.push(Adjcell);
    }
    //check right
    if (grid[row][col+1] != '#' && visited[row][col+1] == 0)
    {
        visited[row][col+1] = 1;
        queueNode Adjcell = { {row, col+1}, curr.dist + 1 };
        q.push(Adjcell);
    }
    //check left
    if (grid[row][col-1] != '#' && visited[row][col-1] == 0)
    {
        visited[row][col-1] = 1;
        queueNode Adjcell = { {row, col-1}, curr.dist + 1 };
        q.push(Adjcell);
    }
    }
    return shortestPath(q, dest);
}


//teleports the player to a new position (if level is not 0) when resetting board
void Dungeon::teleport(int row, int col, char &l)
{
    //loop through until player has a valid position
     
    char pos = grid[row-1][col-1];
    
    while(pos == '#' || pos == 'G' || pos == 'D' || pos == 'S' || pos == 'B')
    {
            row = randInt(1, 17);
            col = randInt(1, 68);
            pos = grid[row-1][col-1];
    }
    
    grid[row-1][col-1] = ' ';
    l = grid[row-1][col-1];
    m_player->setPosition(row-1, col-1);
    grid[row-1][col-1] = '@';
     
}

//set position of a particular character on the grid
void Dungeon::setPos(int r, int c, char s)
{
    grid[r][c] = s;
}

//get player
Player* Dungeon::player() const
{
    return m_player;
}

//find a starting position for the player
void Dungeon::addPlayer(int row, int col, char &l)
{
    char pos = grid[row][col];
    //loop through until player has a valid position
    while(pos == '#' || pos == 'G' || pos == 'D' || pos == 'S' || pos == 'B')
    {
        grid[row][col] = '#';
        row = randInt(1, 17);
        col = randInt(1, 69);
        pos = grid[row][col];
    }
    //set player position on the grid and create a new player
    l = grid[row][col];
    m_player = new Player(this, row, col);
    grid[m_player->getRow()][m_player->getCol()] = '@';
}

//get char at a specific position on the gird
char Dungeon::getPos(int r, int c) const
{
    return grid[r][c];
}

//make sure the player is moving to a spot that isnt the wall
bool Dungeon::checkValid(int r, int c, int dir)
{
    //determine if the space is moveable on
    switch (dir)
    {
    case ARROW_UP:
            if(grid[r-1][c] == '#') return false;
            break;
    case ARROW_DOWN:
            if(grid[r+1][c] == '#') return false; //stay still
            break;
    case ARROW_LEFT:
            if(grid[r][c-1] == '#') return false; //stay still
            break;
    case ARROW_RIGHT:
            if(grid[r][c+1] == '#') return false; //stay still
            break;
    }
    return true;
    
}

//player attack function
void Dungeon::playerAttack(int r, int c)
{
    for(int k = 0; k < m_monster.size(); k++)
    {
        if(m_monster[k]->getRow() == r && m_monster[k]->getCol() == c)
        {
            m_result = m_player->resultOfAttack(m_monster[k]);
            string name = m_monster[k]->name();
            
            //if the monster is dead after a hit
            if(m_monster[k]->getHit() <= 0)
            {
                char last = m_monster[k]->getLast();
                m_monster.erase(m_monster.begin() + k);
                grid[r][c] = last;
                if(name == "Dragon" && grid[r][c] == ' ')
                {
                    //generate a random scroll on the board
                    int result = randInt(0, 4);
                    if(result == 0)
                    {
                        m_object.push_back(new Teleportation(this, r, c));
                        grid[r][c] = '?';
                    }
                    else if(result == 1)
                    {
                        m_object.push_back(new ImproveArmor(this, r, c));
                        grid[r][c] = '?';
                    }
                    else if(result == 2)
                    {
                        m_object.push_back(new RaiseStrength(this, r, c));
                        grid[r][c] = '?';
                    }
                    else if(result == 3)
                    {
                        m_object.push_back(new EnhanceHealth(this, r, c));
                        grid[r][c] = '?';
                    }
                    else if(result == 4)
                    {
                        m_object.push_back(new EnhanceDex(this, r, c));
                        grid[r][c] = '?';
                    }
                }
                if(name == "Snakewoman" && grid[r][c] == ' ')
                {
                    //generate magic fangs with 1/3 prob
                    if(trueWithProbability(.3333) == true)
                    {
                        m_object.push_back(new MagicFangs(this, r, c));
                        grid[r][c] = ')';
                    }
                }
                if(name == "Bogeyman" && grid[r][c] == ' ')
                {
                    //generate magic axe with 1/10 prob
                    if(trueWithProbability(.1) == true)
                    {
                        m_object.push_back(new MagicAxe(this, r, c));
                        grid[r][c] = ')';
                    }
                }
                if(name == "Goblin" && grid[r][c] == ' ')
                {
                    //generate magic fangs or magic axe with 1/3 prob
                    if(trueWithProbability(.3333) == true)
                    {
                        int result = randInt(0, 1);
                        if(result == 0)
                        {
                            m_object.push_back(new MagicFangs(this, r, c));
                            grid[r][c] = ')';
                        }
                        else if(result == 1)
                        {
                            m_object.push_back(new MagicAxe(this, r, c));
                            grid[r][c] = ')';
                        }
                    }
                }
            }
        }
    }
}


//determines the new player of the player - if there is a monster in the direction the player wants to move, attack the monster rather than moving
bool Dungeon::determineNewPosition(int& r, int& c, int dir, char &l)
{
    grid[r][c] = l;
    switch (dir)
    {
    case ARROW_UP:
            if (r <= 1) return false;
            else if (grid[r-1][c] == 'D' || grid[r-1][c] == 'G' || grid[r-1][c] == 'B' || grid[r-1][c] == 'S')
                playerAttack(r-1, c); //attack monster
            else
            {
                //if valid location, set new row/col value
                l = grid[r-1][c];
                r--;
            }
            break;
    case ARROW_DOWN:
            if (r >= 18) return false;
            else if (grid[r+1][c] == 'D' || grid[r+1][c] == 'G' || grid[r+1][c] == 'B' || grid[r+1][c] == 'S')
                playerAttack(r+1, c);
            else
            {
                l = grid[r+1][c];
                r++;
            }
            break;
    case ARROW_LEFT:
            if (c <= 1)
                return false;
            else if (grid[r][c-1] == 'D' || grid[r][c-1] == 'G' || grid[r][c-1] == 'B' || grid[r][c-1] == 'S')
                playerAttack(r, c-1);
            else
            {
                l = grid[r][c-1];
                c--;
            }
            break;
    case ARROW_RIGHT:
            if (c >= 70)
                return false;
            else if (grid[r][c+1] == 'D' || grid[r][c+1] == 'G' || grid[r][c+1] == 'B' || grid[r][c+1] == 'S')
                playerAttack(r, c+1); 
            else
            {
                l = grid[r][c+1];
                c++;
            }
            break;
    default:     return false;
    }
    
    //set player's new position
    m_player->setPosition(r, c);
    grid[m_player->getRow()][m_player->getCol()] = '@';
    return true;
}

//pick up an object
void Dungeon::pickUpObj(int r, int c)
{
    for(int k = 0; k < m_object.size(); k++)
    {
        //find object at specific location
        if(m_object[k]->getRow() == r && m_object[k]->getCol() == c)
        {
            //pick the object up and add it to the player's inventory
            printResult(m_object[k]);
            bool s = m_player->addInventory(m_object[k]);
            if(s == true)
            {
                //erase the object from the vector
                m_object.erase(m_object.begin() + k);
                break;
            }
            if (s == false)
                m_result = "Your knapsack is full; you can't pick that up."; 
            break; 
        }
    }
}

//select the new weapon by calling player class and set result to m_result which displays the string on the display
void Dungeon::selectWeapon(char c)
{
    m_result = m_player->setWeapon(c);
}

//read a scroll and set m_result to display the string on display
void Dungeon::readScroll(char c)
{
    m_result = m_player->readScroll(c); 
}

//pick up an object and set m_result to display the string on display
void Dungeon::printResult(Object* o)
{
    m_result = m_player->resultOfObject(o);
}

//returns result
string Dungeon::getResult() const
{
    return m_result; 
}

//move all monsters in the vector on each turn if moveable
void Dungeon::moveMonster(int smell)
{
    for(int k = 0; k < m_monster.size(); k++)
    {
        //if the monster is asleep, keep still
        if(m_monster[k]->getSleep() > 0)
        {
            m_monster[k]->setSleep(m_monster[k]->getSleep() -1);
        }
        else
        {
            //if a dragon, chance they will regain points
            if(m_monster[k]->name() == "Dragon")
            {
                if(trueWithProbability(.1) == true)
                    m_monster[k]->setHit(m_monster[k]->getHit() + 1);
            }
            //if the monster is next to the player, attack
            if(grid[m_monster[k]->getRow() + 1][m_monster[k]->getCol()] == '@'
               || grid[m_monster[k]->getRow() - 1][m_monster[k]->getCol()] == '@'
               || grid[m_monster[k]->getRow()][m_monster[k]->getCol() + 1 ] == '@'
               || grid[m_monster[k]->getRow()][m_monster[k]->getCol() - 1] == '@')
            {
                m_action = m_monster[k]->resultOfAttack(m_player);//attack player
            }
            else //move monster
            {
                if(m_monster[k]->name() == "Snakewoman")
                {
                    /*
                        //else move closer
                        if((grid[m_monster[k]->getRow()+1][m_monster[k]->getCol()] != '#' && grid[m_monster[k]->getRow()-1][m_monster[k]->getCol()] != '#' && grid[m_monster[k]->getRow()][m_monster[k]->getCol()-1] != '#') || (grid[m_monster[k]->getRow()+1][m_monster[k]->getCol()] != '#' && grid[m_monster[k]->getRow()-1][m_monster[k]->getCol()] != '#' && grid[m_monster[k]->getRow()][m_monster[k]->getCol()+1] != '#') || (grid[m_monster[k]->getRow()+1][m_monster[k]->getCol()] != '#' && grid[m_monster[k]->getRow()][m_monster[k]->getCol()+1] != '#' && grid[m_monster[k]->getRow()][m_monster[k]->getCol()-1] != '#') || (grid[m_monster[k]->getRow()][m_monster[k]->getCol()+1] != '#' && grid[m_monster[k]->getRow()-1][m_monster[k]->getCol()] != '#' && grid[m_monster[k]->getRow()][m_monster[k]->getCol()-1] != '#'))*/
                            moveBorS(m_monster[k]);
                    }
                else if(m_monster[k]->name() == "Goblin")
                {
                    moveGoblin(m_monster[k], smell);
                }
                else if(m_monster[k]->name() == "Bogeyman")
                {
                     moveBorS(m_monster[k]);
                }
            }
        }
    }
}

//check that the monster is moving to a valid position
bool Dungeon::checkValidMonster(int r, int c)
{
    if(grid[r][c] == '#' || grid[r][c] == 'D' || grid[r][c] == 'B' || grid[r][c] == 'S' || grid[r][c] == 'G')
        return false;
    return true;
}

//move the snakewoman or the bogeyman
void Dungeon::moveBorS(Actor* b)
{
    int radius = 3;
    char name = 'S';
    //if monster is a bogeyman, set name and radius accordingly
    if(b->name() == "Bogeyman")
    {
        radius = 5;
        name = 'B';
    }
    
    //look within a radius that the monster can smell
    for(int r = b->getRow() -radius; r < b->getRow() + radius; r++)
    {
        for(int c = b->getCol() - radius; c < b->getCol() + radius; c++)
        {
            //make sure smell never goes out of bounds
            if(r >= 1 && c >= 1 && r <= 17 && c <= 68)
            {
                if(grid[r][c] == '@') //if the player is found
                {
                    //if the player is up from the monster, move it up
                    if(r < b->getRow() && checkValidMonster(b->getRow()-1, b->getCol()))
                    {
                        grid[b->getRow()][b->getCol()] = b->getLast();
                        b->setLast(grid[b->getRow()-1][b->getCol()]);
                        b->setPosition(b->getRow()-1, b->getCol());
                        grid[b->getRow()][b->getCol()] = name;
                    }
                    //move down
                    else if ( r > b->getRow() && checkValidMonster(b->getRow()+1, b->getCol()))
                    {
                        grid[b->getRow()][b->getCol()] = b->getLast();
                        b->setLast(grid[b->getRow()+1][b->getCol()]);
                         b->setPosition(b->getRow()+1, b->getCol());
                        grid[b->getRow()][b->getCol()] = name;
                    }
                    //move left
                    else if(c < b->getCol() &&   checkValidMonster(b->getRow(), b->getCol()-1))
                    {
                        grid[b->getRow()][b->getCol()] = b->getLast();
                        b->setLast(grid[b->getRow()][b->getCol()-1]);
                         b->setPosition(b->getRow(), b->getCol()-1);
                        grid[b->getRow()][b->getCol()] = name;
                    }
                    //move right
                    else if(c > b->getCol() && checkValidMonster(b->getRow(), b->getCol()+1))
                    {
                        grid[b->getRow()][b->getCol()] = b->getLast();
                        b->setLast(grid[b->getRow()][b->getCol()+1]);
                         b->setPosition(b->getRow(), b->getCol()+1);
                        grid[b->getRow()][b->getCol()] = name;
                    }
                }
            }
        }
    }
}

//sets the result that is displayed onto the display
void Dungeon::setResult(string s)
{
    m_result = s; 
}

//add a monster to the grid
void Dungeon::addMonster(int row, int col)
{
    char pos = grid[row][col];
    //loop through until player has a valid position
    while(pos == '#' || pos == '>' || pos == '&' || pos == 'D' || pos == 'G' || pos == 'S' || pos == 'B')
    {
        row = randInt(1, 17);
        col = randInt(1, 69);
        pos = grid[row][col];
    }
    
    //create a vector of monsters
    vector <string>randObject;
    randObject.push_back("Snakewoman");
    randObject.push_back("Goblin");
    randObject.push_back("Bogeyman");
    randObject.push_back("Dragon"); 
    
    string result = "";
    //get a random number depending on what level we are on
    if(m_level == 0 || m_level == 1)
        result = randObject[randInt(0, 1)];
    else if(m_level == 2)
        result = randObject[randInt(0, 2)];
    else
        result = randObject[randInt(0, 3)];
    
    //find result and add that result to the grid and to the game
    if(result == "Snakewoman")
    {
        m_monster.push_back(new Snakewoman(this, row, col));
        grid[row][col] = 'S';
    }
    else if(result == "Goblin")
    {
        m_monster.push_back(new Goblin(this, row, col));
        grid[row][col] = 'G';
    }
    else if(result == "Bogeyman")
    {
        m_monster.push_back(new Bogeyman(this, row, col));
        grid[row][col] = 'B';
    }
    else if(result == "Dragon")
    {
        m_monster.push_back(new Dragon(this, row, col));
        grid[row][col] = 'D';
    }
}

//Add an object to the grid
void Dungeon::addObject(int row, int col)
{
    
    char pos = grid[row][col];
    //loop through until object has a valid position
    while(pos == '#' || pos == 'G' || pos == 'D' || pos == 'S' || pos == 'B' || pos == '>' || pos == '&')
    {
        row = randInt(1, 17);
        col = randInt(1, 69);
        pos = grid[row][col];
    }
    
    //Randomize Object:
    vector<string> randObject;
    randObject.push_back("mace");
    randObject.push_back("short sword");
    randObject.push_back("long sword");
    randObject.push_back("scroll of enhance armor");
    randObject.push_back("scroll of strength");
    randObject.push_back("scroll of enhance health");
    randObject.push_back("scroll of enhance dexterity");
    
    string result = randObject[randInt(0, 6)]; //get a random object
    //add it to the grid/game based on it's name
    if(result == "mace")
    {
        m_object.push_back(new Mace(this, row, col));
        grid[row][col] = ')';
    }
    else if(result == "short sword")
    {
        m_object.push_back(new ShortSword(this, row, col));
        grid[row][col] = ')';
    }
    else if(result == "long sword")
    {
        m_object.push_back(new LongSword(this, row, col));
        grid[row][col] = ')';
    }
    else if(result == "scroll of enhance armor")
    {
        m_object.push_back(new ImproveArmor(this, row, col));
        grid[row][col] = '?';
    }
    else if(result == "scroll of strength")
    {
        m_object.push_back(new RaiseStrength(this, row, col));
        grid[row][col] = '?';
    }
    else if(result == "scroll of enhance health")
    {
        m_object.push_back(new EnhanceHealth(this, row, col));
        grid[row][col] = '?';
    }
    else if(result == "scroll of enhance dexterity")
    {
        m_object.push_back(new EnhanceDex(this, row, col));
        grid[row][col] = '?';
    }
}

//add stairs/exit to the game if level is not the last one
void Dungeon::addStairs(int row, int col)
{
    char pos = grid[row][col];
    //loop through until stairs have a valid position
    while(pos == '#' || pos == 'G' || pos == 'D' || pos == 'S' || pos == 'B' || pos == ')' || pos == '?'|| pos == '@')
    {
        row = randInt(1, 17);
        col = randInt(1, 69);
        pos = grid[row][col];
    }
    
    //add the stairs to the grid
    grid[row][col] = '>';
}

//add golden idol to the game if it is the last level
void Dungeon::addIdol(int row, int col)
{
    char pos = grid[row][col];
    //loop through until player has a valid position
    while(pos == '#' || pos == 'G' || pos == 'D' || pos == 'S' || pos == 'B' || pos == ')' || pos == '?')
    {
        row = randInt(1, 17);
        col = randInt(1, 69);
        pos = grid[row][col];
    }
    grid[row][col] = '&'; 
}

//get level
int Dungeon::getLevel() const
{
    return m_level;
}

//set level
void Dungeon::setLevel(int level)
{
    m_level = level;
}

//check to see if new room coordinates are overlapping
bool Dungeon::valid( int rCorner, int rEnd, int cCorner, int cEnd)
{
    //make sure that the square does not overlap with any previous ones
    for(int r = rCorner; r < rEnd; r++)
    {
        for( int c = cCorner; c < cEnd; c++)
        {
            if(grid[r][c] == ' ')
            {
                return false;
            }
            //make sure that none of it's borders overlap with another
            if(grid[r+1][c] == ' ')
                return false;
            if(grid[r-1][c] == ' ')
                return false;
            if(grid[r][c+1] == ' ')
                return false;
            if(grid[r][c-1] == ' ')
                return false;
        }
    }
    return true;
}

//set up the grid/board
void Dungeon::emptyBoard()
{
    int count = 0; // amount of times the code is looped
       for(int r = 0; r < 18; r ++)
           for(int c = 0; c < 70; c++)
               grid[r][c] = '#';  //fill the board with walls
       
       int n = randInt(4, 7); //generate amount of rooms
       int squares = n;
       while(squares > 0) //keep creating rooms until desired amount
       {
           count ++;
           if(count >50) //break loop if it still hasnt generated enough squares after 50 tries so it doesnt take up too much power
               break;
           //get center of the rectangle
           int rCenter = 0;
           int cCenter = 0;
           rCenter = randInt(4, 13);
           cCenter = randInt(15, 55);
           
           //get length and width
           int l = randInt(4, 9);
           int w = randInt(8, 15);
           
           //get right ending corners
           int rEnd = rCenter + (l/2);
           int cEnd = cCenter + (w / 2);
           
           //get top corner of the room
           int rCorner = rCenter - (l / 2);
           int cCorner = cCenter - (w / 2);
           
           //check it is not overlapping with any other squares
           bool result = valid( rCorner, rEnd, cCorner, cEnd);
           if(result == true)
           {
               //make square
               for(int r = rCorner; r < rEnd; r++)
               {
                   for( int c = cCorner ; c < cEnd; c++)
                   {
                       grid[r][c] = ' ';
                   }
               
               }
               
               //make coordioors
               if(squares != n)
               {
                   bool connect = false;
                   bool connected = false;
                   int r =0;
                   int c =0;
                   
                   //upper left hand corner up
                   if(connected == false)
                   {
                       for(r = rCorner-1; r > 0; r-- )
                       {
                           //if at some point looking straight hits another square, connect em
                           if(grid[r][cCorner] == ' ')
                           {
                               connect = true;
                               break;
                           }
                       }
                       //if there is another room, connect them
                       if(connect == true)
                       {
                           for(int row = rCorner-1; row > r; row--)
                           {
                               grid[row][cCorner] = ' ';
                           }
                           connected = true;
                       }
                       
                   }
                   //upper left hand corner left
                   if(connected == false)
                   {
                       for( c = cCorner-1; c > 0; c--)
                       {
                           if(grid[rCorner][c] == ' ')
                           {
                               connect = true;
                               break;
                           }
                       }
                       if(connect == true)
                       {
                           for(int col = cCorner-1; col > c; col--)
                           {
                               grid[rCorner][col] = ' ';
                           }
                           connected = true;
                       }
                   }
                    
                   //lower right corner
                   if(connected == false)
                   {
                       //for lower right hand corner down
                       for(r = rEnd; r < 17; r++ )
                       {
                           //if at some point looking straight hits another square, connect em
                           if(grid[r][cEnd-1] == ' ')
                           {
                               connect = true;
                               break;
                           }
                       }
                       if(connect == true)
                       {
                           for(int row = rEnd; row < r; row++)
                           {
                               grid[row][cEnd-1] = ' ';
                           }
                           connected = true;
                       }
                   }
                   //for lower right hand corner to the right
                   if(connected == false)
                   {
                       for( c = cEnd; c < 69; c++)
                       {
                           if(grid[rEnd-1][c] == ' ')
                           {
                               connect = true;
                               break;
                           }
                       }
                       if(connect == true)
                       {
                           for(int col = cEnd; col < c; col++)
                           {
                               grid[rEnd-1][col] = ' ';
                           }
                           connected = true;
                       }
                   }
                       
                   //upper right hand corner
                    if(connected == false)
                    {
                        //up
                        for(r = rCorner-1; r > 0; r-- )
                        {
                            if(grid[r][cEnd-1] == ' ')
                            {
                                connect = true;
                                break;
                            }
                        }
                        if(connect == true)
                        {
                            for(int row = rCorner-1; row > r; row--)
                            {
                                grid[row][cEnd-1] = ' ';
                            }
                            connected = true;
                        }
                       
                    }
                   //lower right hand corner
                   if(connected == false)
                   {
                       for( c = cEnd; c < 69; c++)
                       {
                           if(grid[rCorner][c] == ' ')
                           {
                               connect = true;
                               break;
                           }
                       }
                       if(connect == true)
                       {
                           for(int col = cEnd; col < c; col++)
                           {
                               grid[rCorner][col] = ' ';
                           }
                           connected = true;
                       }
                   }
                   
                   //lower left hand corner
                   if(connected == false)
                   {
                       //for lower right hand corner down
                       for(r = rEnd; r < 17; r++ )
                       {
                           //if at some point looking straight hits another square, connect em
                           if(grid[r][cCorner] == ' ')
                           {
                               connect = true;
                               break;
                           }
                       }
                       if(connect == true)
                       {
                           for(int row = rEnd; row < r; row++)
                           {
                               grid[row][cCorner] = ' ';
                           }
                           connected = true;
                       }
                   }
                   
                   //upper left hand corner left
                   if(connected == false)
                   {
                       for( c = cCorner-1; c > 0; c--)
                       {
                           if(grid[rEnd-1][c] == ' ')
                           {
                               connect = true;
                               break;
                           }
                       }
                       if(connect == true)
                       {
                           for(int col = cCorner-1; col > c; col--)
                           {
                               grid[rEnd-1][col] = ' ';
                           }
                           connected = true;
                       }
                   }
                   //if after checking every one its still false, erase the square and just generate a new one
                   if(connected == false)
                   {
                       for(int r = rCorner; r < rEnd; r++)
                       {
                           for( int c = cCorner ; c < cEnd; c++)
                           {
                               grid[r][c] = '#';
                           }
                       
                       }
                       squares++;
                   }
               }
               squares--;
           }
        
               
       }
       
       //fill borders with walls
       for(int r = 0; r < 18; r++)
       {
           grid[r][0] = '#';
           grid[r][69] = '#';
       }
       for(int c = 0; c < 70; c++)
       {
           grid[0][c] = '#';
           grid[17][c] = '#';
       }
}

//print out the display
void Dungeon::display() 
{
    //draw the grid
    clearScreen();

    for (int r = 0; r < 18; r++)
    {
        for(int c = 0; c < 70; c++)
            cout << grid[r][c];
        cout << endl; 
    }
    cout<<endl;
    

    //display stats at the bottom
   cout << "Dungeon Level: " << m_level;
    cout << ", Hit points: " << m_player->getHit();
    cout << ", Armor: " << m_player->getArmor();
    cout << ", Strength: " << m_player->getStrength();
    cout << ", Dexterity: " << m_player->getDex();
    cout << endl;
    
    cout << m_result <<endl;
    cout << m_action <<endl;
    
    
}
