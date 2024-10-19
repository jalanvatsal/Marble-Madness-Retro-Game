#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

// Actor Base Class
class Actor : public GraphObject
{
public:
    
    // should call setvisible in the constructor
    Actor(StudentWorld* sw, int imageID, int startX, int startY, int startDirection);
    
    // do common behavior of checking aliveness status before moving
    // returns -1 if actor is dead, 0 otherwise
    int doSomethingp1();
    
    //pure virtual function as every actor will have their specific implementation
    virtual void doSomething() = 0;
    
    // virtual function as actors who can take damage will override implementation
    virtual void damage(){
        ;
    }
    
    // virtual function as marble will override this
    virtual void push(int dir, int col, int row){
        ;
    }
    
    // return pointer to the current world actors are all in
    StudentWorld * getWorld(){
        return world;
    }
    
    // setter for is_alive data member
    void set_is_alive(bool value){
        is_alive = value;
    }
    
    // getter for is_alive data member
    bool is_actor_alive(){
        return is_alive;
    }
    
    // setter for hit_points data member
    void set_hit_points(int value){
        hit_points = value;
    }
    
    // getter for hit_points data member
    int get_hit_points(){
        return hit_points;
    }
    
    // to discern amongst how valuable a certain goodie is
    // means it is not a Goodie with any utility
    virtual std::string GoodieUtility(){
        return "";
    }
    
    // to discern amongst actors which can be attacked
    virtual bool canBeAttacked(){
        return false;
    };
    
    // to discern amongst actors which can move
    virtual bool canMove(){
        return false;
    };
    
    // to discern amongst actors which is an obstruction
    virtual bool isObstruction(){
        return false;
    };
    
    // to discern amongst actors which have hp
    // -1 indicates that it does not have any hit points (return value should be starting hp for any given actor)
    // REMEMBER that RageBot and Marble have same starting hp
    virtual int hasHealth(){
        return -1;
    };
    
    // to discern amongst actors which are enemies
    virtual bool isEnemy(){
        return false;
    }
    
    // to discern amongst actors which can be picked up
    virtual bool canBeCollected(){
        return false;
    };
    
    // check if actor is needed for exit to appear
    virtual bool needed_for_exit(){
        return false;
    }
    
private:
    // to track Actor's aliveness status
    bool is_alive;
    
    // to track Actor's health (is -1 if they don't have hit points)(starting value is -1(assume no hp))
    int hit_points;
    
    StudentWorld * world;
};

// Avatar Class
class Avatar : public Actor
{
public:
    
    Avatar(StudentWorld* sw,int imageID, int startX, int startY, int startDirection);
    
    virtual void doSomething();
    
    // setter for player pea count
    void set_Pea_Count(int value){
        Pea_Count = value;
    }
    
    // getter for player pea count
    int get_Pea_Count(){
        return Pea_Count;
    }
    
    // override Actor definitions
    virtual bool canBeAttacked(){
        return true;
    }
    // may change this
    virtual bool isObstruction(){
        return true;
    }
    virtual bool canMove(){
        return true;
    }
    virtual int hasHealth(){
        return 20;
    }
    virtual void damage();
    
private:
    int Pea_Count;
    
};

// Robot Class
class Robot : public Actor
{
public:
    Robot(StudentWorld* sw,int imageID, int startX, int startY, int startDirection);
    
    // may make this pure virtual to enforce we cannot make a generic robot
    // check if it needs to rest or is alive
    bool doSomethingCheck();
    
    // this is for the RageBot only, ThiefBot will provide overriding implementation
    virtual void damage();
    
    // getter for tick
    int get_tick(){
        return tick;
    }
    
    // setter for k
    void set_k(int value){
        k = value;
    }
    
    // getter for k
    int get_k(){
        return k;
    }
    
    // as RageBots and Mean ThiefBots both can do this
    // takes in robot's current position as parameters
    // returns true if pea is successfully fired
    bool firePea(int col, int row);
    
    // override Actor definitions
    // these are true for all Robots
    virtual bool canBeAttacked(){
        return true;
    }
    virtual bool canMove(){
        return true;
    }
    virtual bool isObstruction(){
        return true;
    }
    
    virtual bool isEnemy(){
        return true;
    }
    
    // override this in RageBot
    virtual bool canStealGoodies(){
        return true;
    };
    
private:
    // value of tick (which depends on level number)
    int tick;
    // to keep track of when Robot should do something
    int k;

};

// RageBot Derived Class
class RageBot : public Robot
{
public:
    RageBot(StudentWorld* sw,int imageID, int startX, int startY, int startDirection);
    virtual void doSomething();
    
    virtual int hasHealth(){
        return 10;
    }
    virtual bool canStealGoodies(){
        return false;
    }
    
private:

};

// Thiefbot Derived Class
class ThiefBot : public Robot
{
public:
    ThiefBot(StudentWorld* sw, int imageID, int startX, int startY, int startDirection);
    // might remove this might not
    virtual void doSomething(){
        ;
    }
    
    // as both ThiefBots do the same GoodieStealing things
    // true if it steals, false otherwise
    bool handleGoodieStealing(int col, int row);
    
    // as both ThiefBots do the same distanceBeforeTurning things
    void handleTurning();
    
    // as all ThiefBots take damage in essentially same way
    virtual void damage();
    
    // as it can collect goodies as an enemy
    virtual bool canBeCollected(){
        return true;
    }
    
private:
    int distanceBeforeTurning;
    
    // to check if we are distanceBeforeTurning value
    int k;
    
    virtual void doPoints() = 0;
    
    // to track whether thiefbot currently has stolen a goodie
    bool collected_goodie;
    
    std::string stolen_goodie_utility;
};


class ThiefBot_Factory : public Actor
{
public:
    enum ProductType { REGULAR, MEAN };
    ThiefBot_Factory(StudentWorld* sw,int imageID, int startX, int startY, int startDirection, ProductType type);
    virtual void doSomething();
    
    // override Actor definition
    virtual bool isObstruction(){
        return true;
    }
    
private:
    // get production type
    ProductType get_production_type(){
        return production_type;
    }
    // record if it creates regular or mean thiefbots
    ProductType production_type;
};

// Subclass of ThiefBot
class RegularThiefBot : public ThiefBot
{
public:
    RegularThiefBot(StudentWorld* sw,int imageID, int startX, int startY, int startDirection);
    virtual void doSomething();
private:
    virtual int hasHealth(){
        return 5;
    }
    virtual void doPoints();
};

// Subclass of ThiefBot
class MeanThiefBot : public ThiefBot
{
public:
    MeanThiefBot(StudentWorld* sw,int imageID, int startX, int startY, int startDirection);
    virtual void doSomething();
private:
    virtual int hasHealth(){
        return 8;
    }
    virtual void doPoints();
};


// Wall Class
class Wall : public Actor
{
public:
    Wall(StudentWorld* sw, int imageID, int startX, int startY, int startDirection);
    virtual void doSomething();
    
    virtual bool isObstruction(){
        return true;
    }
private:
};

// Exit Class
class Exit : public Actor
{
public:
    Exit(StudentWorld* sw, int imageID, int startX, int startY, int startDirection);
    virtual void doSomething();
    
    virtual bool needed_for_exit(){
        return true;
    }
private:
    bool isVisible;
    bool playedOnce;
};

// Pit Class
class Pit : public Actor
{
public:
    Pit(StudentWorld* sw, int imageID, int startX, int startY, int startDirection);
    virtual void doSomething();
    
    // override Actor definitions
    virtual bool canBeAttacked(){
        return true;
    }
    virtual bool isObstruction(){
        return true;
    }
private:
};

class Marble : public Actor
{
public:
    Marble(StudentWorld* sw, int imageID, int startX, int startY, int startDirection);
    
    virtual void doSomething();
    
    // update marble's + player's position (return true if marble can be pushed, false otherwise)
    void push(int dir, int col, int row);
    
    
    // override Actor definitions
    virtual bool canBeAttacked(){
        return true;
    }
    
    virtual bool canMove(){
        return true;
    }

    virtual bool isObstruction(){
        // as we handle cases where it is not obstruction in Avatar's move()
        return true;
    }
    virtual int hasHealth(){
        return 10;
    }
    
    virtual void damage();
    
private:
    
};

// Pea Class
class Pea : public Actor
{
public:
    Pea(StudentWorld* sw, int imageID, int startX, int startY, int startDirection);
    virtual void doSomething();
    
    // override Actor definition
    virtual bool canMove(){
        return true;
    }
private:
    
};

// Goodies Class (base class for all goodies (extra life, restore health, ammo, and crystal))
// need Goodies class to implement functions that handle common behavior during tick
class Goodies : public Actor
{
public:
    Goodies(StudentWorld* sw, int imageID, int startX, int startY, int startDirection);
    
    virtual void doSomething(); // in which we do the things below
    
    // for increasing score (to be used in doPoints)
    void increaseScore(int increase);
    
    // this property is true for all goodies
    virtual bool canBeCollected(){
        return true;
    }

private:
    
    // handle updating points
    virtual void doPoints() = 0;
    
    // for handling specific goodie effects
    virtual void doDifferentiatedStuff() = 0;

};

// Different Goodies
class Extra_Life_Goodie : public Goodies
{
public:
    Extra_Life_Goodie(StudentWorld* sw, int imageID, int startX, int startY, int startDirection);
    
    virtual std::string GoodieUtility(){
        return "High";
    }
    
private:
    virtual void doPoints();
    virtual void doDifferentiatedStuff();
};


class Restore_Health_Goodie : public Goodies
{
public:
    Restore_Health_Goodie(StudentWorld* sw, int imageID, int startX, int startY, int startDirection);
    
    virtual std::string GoodieUtility(){
        return "Medium";
    }
    
private:
    virtual void doPoints();
    virtual void doDifferentiatedStuff();
};

class Ammo_Goodie : public Goodies
{
public:
    Ammo_Goodie(StudentWorld* sw, int imageID, int startX, int startY, int startDirection);
    
    virtual std::string GoodieUtility(){
        return "Low";
    }
    
private:
    virtual void doPoints();
    virtual void doDifferentiatedStuff();
};

class Crystal : public Goodies
{
public:
    Crystal(StudentWorld* sw, int imageID, int startX, int startY, int startDirection);
    
    virtual bool needed_for_exit(){
        return true;
    }
    
private:
    virtual void doPoints();
    virtual void doDifferentiatedStuff();
    
};


#endif // ACTOR_H_



