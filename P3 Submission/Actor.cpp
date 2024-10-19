#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//Actor class
Actor::Actor(StudentWorld* sw, int imageID, int startX, int startY, int startDirection)
:GraphObject(imageID, startX, startY, startDirection), world(sw), is_alive(true)
{
    // make all actors visible on screen
    setVisible(true);
    
    //set all actors to have no hit points in the beginning (starting value = -1)
    set_hit_points(-1);
}

int Actor::doSomethingp1(){
    
    if(!is_actor_alive()){
        return -1;
    }
    return 0;
}

// Avatar Class
Avatar::Avatar(StudentWorld* sw,int imageID, int startX, int startY, int startDirection)
:Actor(sw,imageID,startX,startY,startDirection)
{
    // initialize starting ammo
    Pea_Count = 20;
    
    // initialize starting health
    set_hit_points(20);
}


void Avatar::doSomething(){
        
    int ch;
    if (getWorld()->getKey(ch))
    {
        // user hit a key this tick!
        switch (ch)
        {
            case KEY_PRESS_LEFT:
                
                setDirection(left);
                
                // check if the adjacent square is a marble, if it is then check if it can be pushed, it it can be pushed, then push the marble and then return from the function
                // if it cannot be pushed then dont do anything, because it will be an obstruction
                if(getWorld() -> isSwallowable(getX() - 1, getY())){
                    if(getWorld() -> canBePushed('l', getX() - 1, getY())){
                        // here we will call callPushOnmarble passing in the marble's location
                        getWorld() -> callPushOrSetAliveOnMarble(left, getX() - 1, getY(), 'p');
                        
                        // now we should move the player to marble's old location
                        moveTo(getX() - 1, getY());
                    }
                }
                
                if(getWorld() -> obstructionPresent(getX() - 1,getY())){
                    break;
                } else {
                    moveTo(getX() - 1,getY());
                }
                break;
            case KEY_PRESS_RIGHT:
                
                setDirection(right);
                
                
                // check if the adjacent square is a marble, if it is then check if it can be pushed, it it can be pushed, then push the marble and then return from the function
                // if it cannot be pushed then dont do anything, because it will be an obstruction
                if(getWorld() -> isSwallowable(getX() + 1, getY())){
                    if(getWorld() -> canBePushed('r', getX() + 1, getY())){
                        // here we will call callPushOnmarble passing in the marble's location
                        getWorld() -> callPushOrSetAliveOnMarble(right, getX() + 1, getY(), 'p');
                        
                        // now we should move the player to marble's old location
                        moveTo(getX() + 1, getY());
                    }
                }

        
                if(getWorld() -> obstructionPresent(getX() + 1,getY())){
                    break;
                } else {
                    moveTo(getX() + 1,getY());
                }
                break;
            case KEY_PRESS_UP:
                
                setDirection(up);
                
                
                // check if the adjacent square is a marble, if it is then check if it can be pushed, it it can be pushed, then push the marble and then return from the function
                // if it cannot be pushed then dont do anything, because it will be an obstruction
                if(getWorld() -> isSwallowable(getX(), getY() + 1)){
                    if(getWorld() -> canBePushed('u', getX(), getY() + 1)){
                        // here we will call callPushOnmarble passing in the marble's location
                        getWorld() -> callPushOrSetAliveOnMarble(up, getX(), getY() + 1, 'p');
                        
                        // now we should move the player to marble's old location
                        moveTo(getX(), getY() + 1);
                    }
                }

                if(getWorld() -> obstructionPresent(getX(),getY() + 1)){
                    break;
                } else {
                    moveTo(getX(),getY() + 1);
                }
                break;
            case KEY_PRESS_DOWN:

                setDirection(down);
                
                // check if the adjacent square is a marble, if it is then check if it can be pushed, it it can be pushed, then push the marble and then return from the function
                // if it cannot be pushed then dont do anything, because it will be an obstruction
                if(getWorld() -> isSwallowable(getX(), getY() - 1)){
                    if(getWorld() -> canBePushed('d', getX(), getY() - 1)){
                        // here we will call callPushOnmarble passing in the marble's location
                        getWorld() -> callPushOrSetAliveOnMarble(down, getX(), getY() - 1, 'p');
                        
                        // now we should move the player to marble's old location
                        moveTo(getX(), getY() - 1);
                    }
                }

                if(getWorld() -> obstructionPresent(getX(),getY() - 1)){
                    break;
                } else {
                    moveTo(getX(),getY() - 1);
                }
                break;
            case KEY_PRESS_ESCAPE:
                // set the state of the player to dead, so move() will remove player from game and end it
                set_is_alive(false);
                getWorld() -> playSound(SOUND_PLAYER_DIE);
                getWorld() -> decLives();
                break;
            case KEY_PRESS_SPACE:
                // implement this later (as well as other key options!)
                if(Pea_Count >= 1){
                    
                    int dir = getDirection();
                    
                    int new_col = getX();
                    int new_row = getY();
                    
                    switch (dir) {
                        case left:
                            new_col--;
                            break;
                        case right:
                            new_col++;
                            break;
                        case up:
                            new_row++;
                            break;
                        case down:
                            new_row--;
                            break;
                        default:
                            break;
                    }
                    // let StudentWorld know that it should animate a pea object
                    getWorld() -> addPea(new_col, new_row, getDirection());
                    // decrement ammo count
                    Pea_Count--;
                    // play sound
                    getWorld() -> playSound(SOUND_PLAYER_FIRE);
                }
                break;
                // etc…
        }
    }
    
}

void Avatar::damage(){
    
    // decrement hit points
    set_hit_points(get_hit_points() - 2);
    
    if(get_hit_points() > 0){
        getWorld() -> playSound(SOUND_PLAYER_IMPACT);
    } else if(get_hit_points() <= 0){
        set_is_alive(false);
        // decrement the lives of the player
        if(getWorld() -> getLives() >= 1){
            getWorld() -> decLives();
        }
        getWorld() -> playSound(SOUND_PLAYER_DIE);
    }
}

// Robots + Robot Subclasses implementations
Robot::Robot(StudentWorld* sw,int imageID, int startX, int startY, int startDirection)
:Actor(sw,imageID,startX,startY,startDirection)
{
    // do nothing else really
    int num = (28 - (getWorld() -> getLevel()));
    int denom = 4;
    int ticks = (num / denom) - 1;
    
    if(ticks < 3){
        ticks = 3;
    }
    
    // initialize the tick member variable
    tick = ticks;
    
    // initialize k member variable
    k = 0;
}

bool Robot::doSomethingCheck(){
    
    // if dead then return immediately
    if(doSomethingp1() == -1){
        return true;
    }
    
    // otherwise update k-count and then return
    if(get_k() < get_tick()){
        set_k(get_k() + 1);
        return true;
    }
    
    return false;
}

bool Robot::firePea(int col, int row){
    
    // if the robot and avatar are horizontally aligned
    if(getWorld() -> getAvatar() -> getY() == getY()){
        
        bool facing_same_way = false;
        if(getWorld() -> getAvatar() -> getX() > getX()){
            if(getDirection() == right){
                facing_same_way = true;
            }
        } else if(getX() > getWorld() -> getAvatar() -> getX()){
            if(getDirection() == left){
                facing_same_way = true;
            }
        }
        
        // if they are facing each other and there is a clear shot
        if(facing_same_way){
            
            if(getWorld() -> isClearShot('h', getX(), getY())){
                //set location of where pea should be spawned (new_col, getY()) // as it should be in same row
                int new_col = getX();
                switch (getDirection()) {
                    case left:
                        new_col--;
                        break;
                    case right:
                        new_col++;
                        break;
                    default:
                        break;
                }
                
                // add a pea into the game
                getWorld() -> addPea(new_col, getY(), getDirection());
                // play sound effect
                getWorld() -> playSound(SOUND_ENEMY_FIRE);
                
                return true;
            }
            
        }
        // if the robot and avatar are vertically aligned
    } else if(getWorld() -> getAvatar() -> getX() == getX()){
        
        // check if they are facing same way
        bool facing_same_way = false;
        if(getWorld() -> getAvatar() -> getY() > getY()){
            if(getDirection() == up){
                facing_same_way = true;
            }
        } else if(getY() > getWorld() -> getAvatar() -> getY()){
            if(getDirection() == down){
                facing_same_way = true;
            }
        }
        
        // if their cols are the same and facing each other and there is a clear shot
        if(facing_same_way){
            
            if(getWorld() -> isClearShot('v', getX(), getY())){
                
                //get location of where pea should be spawned
                int new_row = getY();
                switch (getDirection()) {
                    case up:
                        new_row++;
                        break;
                    case down:
                        new_row--;
                        break;
                    default:
                        break;
                }
                
                // add a pea into the game
                getWorld() -> addPea(getX(), new_row, getDirection());
                // play sound effect
                getWorld() -> playSound(SOUND_ENEMY_FIRE);
                
                return true;
            }
            
        }
    }
    
    // means firing a pea was unsuccessful (either there is obstacle or facing in diff ways)
    return false;
}

void Robot::damage(){
    
    set_hit_points(get_hit_points() - 2);
    
    if(get_hit_points() > 0){
        getWorld() -> playSound(SOUND_ROBOT_IMPACT);
    } else {
        // set state to dead
        set_is_alive(false);
        
        getWorld() -> playSound(SOUND_ROBOT_DIE);
        
        getWorld() -> increaseScore(100);
        
    }
    
}

RageBot::RageBot(StudentWorld* sw,int imageID, int startX, int startY, int startDirection)
:Robot(sw,imageID,startX,startY,startDirection)
{
    // set hit points
    set_hit_points(10);
}

void RageBot::doSomething(){
    
    // check if we are supposed to rest or are not alive
    if(Robot::doSomethingCheck()){
        return;
    }
    
    // reset k to be 0 again (as we are doing something)
    set_k(0);
    
    // call Robot's firePea function
    // if its true, then do nothing else in this tick
    if(Robot::firePea(getX(), getY())){
        return;
    }
    
    // at this point, RageBot should try and move to adjacent square in its specified direction (or switch dir)
    int new_col = getX();
    int new_row = getY();
    
    switch (getDirection()) {
        case left:
            new_col--;
            break;
        case right:
            new_col++;
            break;
        case up:
            new_row++;
            break;
        case down:
            new_row--;
            break;
        default:
            break;
    }
    
    if(getWorld() -> obstructionPresent(new_col, new_row)){
        
        // just switch to opposite direction
        
        switch (getDirection()) {
            case left:
                setDirection(right);
                break;
            case right:
                setDirection(left);
                break;
            case up:
                setDirection(down);
                break;
            case down:
                setDirection(up);
                break;
            default:
                break;
        }
        
    } else {
        moveTo(new_col, new_row);
    }
    
}

ThiefBot_Factory::ThiefBot_Factory(StudentWorld* sw,int imageID, int startX, int startY, int startDirection, ProductType type)
:Actor(sw, imageID, startX, startY, startDirection)
{
    // specify which type of thiefbot this factory manufactures
    production_type = type;
}

void ThiefBot_Factory::doSomething(){
    
    if(get_production_type() == REGULAR){
        getWorld() -> shouldBeBorn(getX(), getY(), 'r');
    } else if(get_production_type() == MEAN){
        getWorld() -> shouldBeBorn(getX(), getY(), 'm');
    }
}

ThiefBot::ThiefBot(StudentWorld* sw,int imageID, int startX, int startY, int startDirection)
:Robot(sw,imageID,startX,startY,startDirection)
{
    // indicate that thiefbot currently has not collected a goodie
    collected_goodie = false;
    
    // indicate that there is no stolen goodie, so there is no stolen goodie utility to measure
    stolen_goodie_utility = "";
    
    distanceBeforeTurning = randInt(1, 6);
    k = 0;
}

void ThiefBot::handleTurning(){
    
    if(k < distanceBeforeTurning){
        
        int new_col = getX();
        int new_row = getY();
        
        switch (getDirection()) {
            case left:
                new_col--;
                break;
            case right:
                new_col++;
                break;
            case up:
                new_row++;
                break;
            case down:
                new_row--;
                break;
            default:
                break;
        }
        
        if(!getWorld() -> obstructionPresent(new_col, new_row)){
            moveTo(new_col, new_row);
            k++;
            return;
        } else {
            // encountered an obstruction
            // choose new value for distanceBeforeTurning
            distanceBeforeTurning = randInt(1, 6);
            
            // choose random dir
            int new_dir = randInt(1, 4);
            switch (new_dir) {
                case 1:
                    new_dir = right;
                    break;
                case 2:
                    new_dir = left;
                    break;
                case 3:
                    new_dir = up;
                    break;
                case 4:
                    new_dir = down;
                    break;
                default:
                    break;
            }
            
            // check 4 dirs
            bool moved = false;
            int temp_dir = new_dir;
            for(int i = 0; i < 4; i++){
                
                int new_col_temp = getX();
                int new_row_temp = getY();
                
                switch (temp_dir) {
                    case left:
                        new_col_temp--;
                        break;
                    case right:
                        new_col_temp++;
                        break;
                    case up:
                        new_row_temp++;
                        break;
                    case down:
                        new_row_temp--;
                        break;
                    default:
                        break;
                }
                
                if(!getWorld() -> obstructionPresent(new_col_temp, new_row_temp)){
                    // change direction of Thiefbot
                    switch (temp_dir) {
                        case right:
                            setDirection(right);
                            break;
                        case left:
                            setDirection(left);
                            break;
                        case up:
                            setDirection(up);
                            break;
                        case down:
                            setDirection(down);
                            break;
                        default:
                            break;
                    }
                    
                    moveTo(new_col_temp, new_row_temp);
                    k++;
                    moved = true;
                    return;
                }
                
                // rotate direction, 90 degrees clockwise
                switch (temp_dir) {
                    case right:
                        temp_dir = down;
                        break;
                    case left:
                        temp_dir = up;
                        break;
                    case up:
                        temp_dir = right;
                        break;
                    case down:
                        temp_dir = left;
                        break;
                    default:
                        break;
                }
                
            }
            
            if(!moved){
                setDirection(new_dir);
                return;
            }
            
        }
    } else {
        
        // we already went distanceBeforeTurning steps
        // choose new value for distanceBeforeTurning
        distanceBeforeTurning = randInt(1, 6);
        
        // reset k
        k = 0;
        
        // choose random dir
        int new_dir = randInt(1, 4);
        switch (new_dir) {
            case 1:
                new_dir = right;
                break;
            case 2:
                new_dir = left;
                break;
            case 3:
                new_dir = up;
                break;
            case 4:
                new_dir = down;
                break;
            default:
                break;
        }
        
        // check 4 dirs
        bool moved = false;
        int temp_dir = new_dir;
        for(int i = 0; i < 4; i++){
            
            int new_col_temp = getX();
            int new_row_temp = getY();
            
            switch (temp_dir) {
                case left:
                    new_col_temp--;
                    break;
                case right:
                    new_col_temp++;
                    break;
                case up:
                    new_row_temp++;
                    break;
                case down:
                    new_row_temp--;
                    break;
                default:
                    break;
            }
            
            if(!getWorld() -> obstructionPresent(new_col_temp, new_row_temp)){
                // change direction of Thiefbot
                switch (temp_dir) {
                    case right:
                        setDirection(right);
                        break;
                    case left:
                        setDirection(left);
                        break;
                    case up:
                        setDirection(up);
                        break;
                    case down:
                        setDirection(down);
                        break;
                    default:
                        break;
                }
                
                moveTo(new_col_temp, new_row_temp);
                k++;
                moved = true;
                return;
            }
            
            // rotate direction, 90 degrees clockwise
            switch (temp_dir) {
                case right:
                    temp_dir = down;
                    break;
                case left:
                    temp_dir = up;
                    break;
                case up:
                    temp_dir = right;
                    break;
                case down:
                    temp_dir = left;
                    break;
                default:
                    break;
            }
            
        }
        
        if(!moved){
            setDirection(new_dir);
            return;
        }
        
    }

}

void ThiefBot::damage(){
    
    set_hit_points(get_hit_points() - 2);
    
    if(get_hit_points() > 0){
        getWorld() -> playSound(SOUND_ROBOT_IMPACT);
    } else {
        
        if(collected_goodie){
            getWorld() -> spawnGoodie(getX(), getY(), stolen_goodie_utility);
        }
        
        // set state to dead
        set_is_alive(false);
        
        getWorld() -> playSound(SOUND_ROBOT_DIE);
        
        //increase score appropriately depending on Mean or Regular ThiefBot
        doPoints();
        
    }
}

// handle the functionality of Thiefbot eating goodie
bool ThiefBot::handleGoodieStealing(int col, int row){
    // first we need to check if Thiefbot is on the same square as a goodie
    std::string s = "";
    if(getWorld() -> sameSquareAsGoodie(getX(), getY(), s)){
        
        // then we know that thiefbot is on same square as a goodie
        int random = 10;
        // simulate a 1 in 10 chance of picking up goodie
        if(random == randInt(1, 10) && !collected_goodie){
            
            // record that thiefbot currently has a goodie
            collected_goodie = true;
            
            // save data of which type of goodie thiefbot has stolen
            stolen_goodie_utility = s;
            
            // pick up specific Goodie object
            getWorld() -> pickUpGoodie(getX(), getY(), stolen_goodie_utility);
            
            // as thiefbot has stolen a goodie
            return true;
        }
    }
    
    return false;
}


RegularThiefBot::RegularThiefBot(StudentWorld* sw,int imageID, int startX, int startY, int startDirection)
:ThiefBot(sw,imageID,startX,startY,startDirection)
{
    // initialize starting hp
    set_hit_points(5);
}

void RegularThiefBot::doPoints(){
    getWorld() -> increaseScore(10);
}

void RegularThiefBot::doSomething(){
    
    if(doSomethingp1() == -1){
        return;
    }
    
    // check if we are supposed to rest or are not alive
    if(Robot::doSomethingCheck()){
        return;
    }
    
    // reset k to be 0 again (as we are doing something)
    set_k(0);
    
    if(handleGoodieStealing(getX(), getY())){
        return;
    }
    
    handleTurning();
    
}


MeanThiefBot::MeanThiefBot(StudentWorld* sw,int imageID, int startX, int startY, int startDirection)
:ThiefBot(sw,imageID,startX,startY,startDirection)
{
    // initialize starting hp
    set_hit_points(8);
}

void MeanThiefBot::doPoints(){
    getWorld() -> increaseScore(20);
}

void MeanThiefBot::doSomething(){
    
    if(doSomethingp1() == -1){
        return;
    }
    
    // check if we are supposed to rest or are not alive
    if(Robot::doSomethingCheck()){
        return;
    }
    
    // reset k to be 0 again (as we are doing something)
    set_k(0);
    
    if(Robot::firePea(getX(), getY())){
        return;
    }
    
    if(handleGoodieStealing(getX(), getY())){
        return;
    }
    
    handleTurning();
    
}


// Wall Class
Wall::Wall(StudentWorld* sw, int imageID, int startX, int startY, int startDirection)
:Actor(sw, imageID, startX, startY, startDirection)
{
    // do nothing else really as of now
}

// Wall doesn't do anything
void Wall::doSomething(){
    ;
}

// Exit Class
Exit::Exit(StudentWorld* sw, int imageID, int startX, int startY, int startDirection)
:Actor(sw, imageID, startX, startY, startDirection)
{
    setVisible(false);
    isVisible = false;
    playedOnce = false;
}

void Exit::doSomething(){
    
    // if all crystals are collected and the exit is not yet visible
    if((getWorld() -> allCrystalsCollected()) && (!isVisible) && (!playedOnce)){
        getWorld() -> playSound(SOUND_REVEAL_EXIT);
        setVisible(true);
        isVisible = true;
        playedOnce = true;
    } else if(getWorld() -> getAvatar() -> getX() == getX() && getWorld() -> getAvatar() -> getY() == getY() && (isVisible)){
        getWorld() -> playSound(SOUND_FINISHED_LEVEL);
        // for finishing a level
        getWorld() -> increaseScore(2000);
        // inform the studentWorld object that the current level has been completed (i.e set exit state to dead)
        set_is_alive(false);
        // check for bonus points and allocate them accordingly (they are allocated in move())
    }
}

// Pit Class
Pit::Pit(StudentWorld* sw, int imageID, int startX, int startY, int startDirection)
:Actor(sw,imageID, startX, startY, startDirection)
{
    // do nothing else really
}

void Pit::doSomething(){
    
    int is_pit_alive = doSomethingp1();
    if(is_pit_alive == -1){
        return;
    }
    
    // check if marble is on top of pit
    if(getWorld() -> isSwallowable(getX(), getY())){
        //change pit's aliveness state
        set_is_alive(false);
        //change marble's aliveness state
        getWorld() -> callPushOrSetAliveOnMarble(left, getX(), getY(), 's');
    }
    
}

// Marble Class
Marble::Marble(StudentWorld* sw, int imageID, int startX, int startY, int startDirection)
:Actor(sw,imageID, startX, startY, startDirection)
{
    // set starting health
    set_hit_points(10);
}

void Marble::doSomething(){
    // Marble doesn't do anything (it's a marble)
}

void Marble::push(int dir, int col, int row){
     // takes in marble's current location as parameters
    
    // these will be updated based on dir and reflect where the marble will move
    int new_col = col;
    int new_row = row;
    
    switch (dir) {
        case left:
            new_col--;
            break;
        case right:
            new_col++;
            break;
        case up:
            new_row++;
            break;
        case down:
            new_row--;
            break;
        default:
            break;
    }
    
    // move the marble
    moveTo(new_col, new_row);
    
}

void Marble::damage(){
    // decrement hit points
    set_hit_points(get_hit_points() - 2);
    
    // set state to dead
    if(get_hit_points() <= 0){
        set_is_alive(false);
    }
}

// Pea Implementation
Pea::Pea(StudentWorld* sw, int imageID, int startX, int startY, int startDirection)
: Actor(sw, imageID, startX, startY, startDirection)
{
    // do nothing else really
}


void Pea::doSomething(){
    
    // this means pea is dead
    if(doSomethingp1() == -1){
        return;
    }
    
    // check for same square as robot, marble, or avatar
    bool check1 = getWorld() -> checkPeaCollision(1, getX(), getY());
    // if we hit a robot, marble, or avatar
    if(check1){
        set_is_alive(false);
        return;
    }
    
    bool check2 = getWorld() -> checkPeaCollision(2, getX(), getY());
    // we hit a Wall or Thiefbot factory
    if(check2){
        set_is_alive(false);
        return;
    }
    
    // at this point Pea could or could not be on the same square as some misc actor, we don't care for the distinction
    // we must move the pea 1 step regardless
    int dir = getDirection();
    
    int new_col = getX();
    int new_row = getY();
    
    switch (dir) {
        case left:
            new_col--;
            break;
        case right:
            new_col++;
            break;
        case up:
            new_row++;
            break;
        case down:
            new_row--;
            break;
        default:
            break;
    }
    // move the pea in specified direction
    moveTo(new_col, new_row);
    
    // now repeat some steps
    // check for same square as robot, marble, or avatar
    bool check3 = getWorld() -> checkPeaCollision(1, getX(), getY());
    // if we hit a robot, marble, or avatar
    if(check3){
        set_is_alive(false);
        return;
    }
    
    bool check4 = getWorld() -> checkPeaCollision(2, getX(), getY());
    // we hit a Wall or Thiefbot factory
    if(check4){
        set_is_alive(false);
        return;
    }
    
}

// Goodies implementations
Goodies::Goodies(StudentWorld* sw, int imageID, int startX, int startY, int startDirection)
:Actor(sw, imageID, startX, startY, startDirection)
{
    // need no other initialization really
}

void Goodies::increaseScore(int increase){
    getWorld() -> increaseScore(increase);
}

// to do the common things for all goodies
void Goodies::doSomething(){
    
    // could change this to doSomethingp1() usage
    if(!is_actor_alive()){
        return;
    }
    
    // checking if player and goodie are on the same square
    if(getWorld() -> getAvatar() -> getX() == getX() && getWorld() -> getAvatar() -> getY() == getY()){
        
        // handle points first
        doPoints();
        
        // change aliveness state of goodie
        set_is_alive(false);
        
        //play common sound
        getWorld() -> playSound(SOUND_GOT_GOODIE);
        
        //do specific goodie effect
        doDifferentiatedStuff();
    }
}

Extra_Life_Goodie::Extra_Life_Goodie(StudentWorld* sw, int imageID, int startX, int startY, int startDirection)
:Goodies(sw, imageID, startX, startY, startDirection)
{
    // need no other initialization really
}

// fill in later (just make it compile right now)
void Extra_Life_Goodie::doPoints(){
    
    increaseScore(1000);
}


void Extra_Life_Goodie::doDifferentiatedStuff(){
    
    getWorld() -> incLives();
}


Restore_Health_Goodie::Restore_Health_Goodie(StudentWorld* sw, int imageID, int startX, int startY, int startDirection)
:Goodies(sw, imageID, startX, startY, startDirection)
{
    // need no other initialization really
}

// fill in later (just make it compile right now)
void Restore_Health_Goodie::doPoints(){
    
    increaseScore(500);
}


void Restore_Health_Goodie::doDifferentiatedStuff(){
    
    // restore the player's health...
    getWorld() -> getAvatar() -> set_hit_points(20);
}


Ammo_Goodie::Ammo_Goodie(StudentWorld* sw, int imageID, int startX, int startY, int startDirection)
:Goodies(sw, imageID, startX, startY, startDirection)
{
    // need no other initialization really
}

// fill in later (just make it compile right now)
void Ammo_Goodie::doPoints(){
    
    increaseScore(100);
    
}

void Ammo_Goodie::doDifferentiatedStuff(){
    
    // refill ammo...(avatar's pea count)
    getWorld() -> getAvatar() -> set_Pea_Count(20);
}

Crystal::Crystal(StudentWorld* sw, int imageID, int startX, int startY, int startDirection)
:Goodies(sw, imageID, startX, startY, startDirection)
{
    // need no other initialization really
}

// fill in later (just make it compile right now)
void Crystal::doPoints(){
    
    increaseScore(50);
    
}

void Crystal::doDifferentiatedStuff(){
    
    //do nothing (doesn't have any "special effects")
}

