#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>

class Actor;
class Avatar;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    // factory's location is passed in as parameters, if count < 3 and no thiefbot on same square then dynamically allocate a new thiefbot of the type the factory manufactures return true, else return false
    bool shouldBeBorn(int col, int row, char prod_type);
    
    // return true, if thiefbot is on the same square as a goodie (except crystals)
    bool sameSquareAsGoodie(int col, int row, std::string &s);
    
    // spawn goodie
    void spawnGoodie(int col, int row, std::string utility);
    
    // for thiefbot to pick up a goodie
    void pickUpGoodie(int col, int row, std::string utility);
    
    // dynamically allocate a new pea object into the game
    void addPea(int startX, int startY, int startDirection);
    
    // Pea's location is passed in
    // returns true if it collides with something with health, false indicates it should keep moving
    bool checkPeaCollision(int step, int col, int row);
    
    // will check if there is a clear shot to the pplayer without (walls, marbles, robots, or robot factories)
    // takes in robot's location as parameters
    bool isClearShot(char dir, int col, int row);
    
    // check for obstructing objects in some coordinate, e.g ones that player cannot be on same square as
    bool obstructionPresent(int col, int row);
    
    // check if there is a marble at that location
    bool isSwallowable(int col, int row);
    
    // check if a marble can be pushed in a certain dir
    bool canBePushed(char dir, int col, int row);
    
    // for the player to push on marble
    void callPushOrSetAliveOnMarble(int dir, int col, int row, char func);
    
    // check if all crystals are collected, we will then reveal exit
    bool allCrystalsCollected();
    
    // check if avatar completed current level
    bool theAvatarCompletedTheCurrentLevel();
    
    // get pointer to the avatar in the world
    Avatar * getAvatar(){
        return avatar;
    }
    
    
private:
    // container of all actors except avatar
    std::vector<Actor*> actors;
    
    // unique pointer to the singular avatar
    Avatar * avatar;
    
    // load a current level, for init function's use (return 0 for success)
    int loadLevel();
    
    // keep track of level bonus
    int bonus;
    
    // decrement bonus every tick
    void reduceLevelBonusByOne(){
        if(bonus > 0){
            bonus -= 1;
        }
    };
    
    std::string format_text(int score, int level, int lives, int hp, int ammo, int bonus);
    
    void setDisplayText();
    
};

#endif // STUDENTWORLD_H_






