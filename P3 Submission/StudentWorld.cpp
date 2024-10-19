#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include<sstream>
#include<iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), bonus(1000)
{
    
}

int StudentWorld::init()
{
    loadLevel();
    // reset bonus for each level
    bonus = 1000;
    return GWSTATUS_CONTINUE_GAME;
}


StudentWorld:: ~StudentWorld(){
    
    // get rid of actors in vector in reverse order
    for(int k = actors.size() - 1; k >= 0; k--){
        delete actors[k];
    }
    
    // now clean up pointers in vector
    while (!actors.empty()) {
        actors.erase(actors.begin());
    }
}

bool StudentWorld::isSwallowable(int col, int row){
    
    for(int k = 0; k < actors.size(); k++){
        
        int x = actors[k] -> getX();
        int y = actors[k] -> getY();
        
        // check if we have a marble right next to the player in specified dir
        if(actors[k] -> hasHealth() == 10 && (!actors[k] -> isEnemy()) && x == col && y == row){
            return true;
        }
        
    }
    
    return false;
    
}

bool StudentWorld::sameSquareAsGoodie(int col, int row, std::string &s){
    
    for(int k = 0; k < actors.size(); k++){
        
        int x = actors[k] -> getX();
        int y = actors[k] -> getY();
        
        // we may change how we identify a marble later (right now, doing with 10 starting hit points)
        // check if we have a marble right next to the player in specified dir
        if(col < VIEW_HEIGHT && row < VIEW_WIDTH && !actors[k] -> isEnemy() && actors[k] -> canBeCollected() && !actors[k] -> needed_for_exit() && x == col && y == row){
            
            // at this point we know we have a goodie (excluding crystals)
            
            // distinguish what kind of goodie we are dealing with and record this data into string s
            if(actors[k] -> GoodieUtility() == "High"){
                s = "High";
            } else if(actors[k] -> GoodieUtility() == "Medium"){
                s = "Medium";
            } else if(actors[k] -> GoodieUtility() == "Low"){
                s = "Low";
            }
            
            return true;
            
        }
        
    }
    
    // no goodie was found on same square
    return false;
    
}

void StudentWorld::pickUpGoodie(int col, int row, std::string utility){
    
    for(int k = 0; k < actors.size(); k++){
        
        int x = actors[k] -> getX();
        int y = actors[k] -> getY();
        
        if(col < VIEW_HEIGHT && row < VIEW_WIDTH && actors[k] -> GoodieUtility() == utility && x == col && y == row){
            // kill the goodie object
            actors[k] -> set_is_alive(false);
            // play sound
            playSound(SOUND_ROBOT_MUNCH);
        }
        
    }
}

void StudentWorld::spawnGoodie(int col, int row, std::string utility){
    
    if(utility == "High"){
        Extra_Life_Goodie * extra = new Extra_Life_Goodie(this,IID_EXTRA_LIFE, col, row, GraphObject::none);
        actors.push_back(extra);
    } else if(utility == "Medium"){
        Restore_Health_Goodie * restore = new Restore_Health_Goodie(this,IID_RESTORE_HEALTH, col, row, GraphObject::none);
        actors.push_back(restore);
    } else if(utility == "Low"){
        Ammo_Goodie * ammo = new Ammo_Goodie(this, IID_AMMO, col, row, GraphObject::none);
        actors.push_back(ammo);
    }
}

bool StudentWorld::shouldBeBorn(int col, int row, char prod_type){
    
    // keep track of how many thiefbots are encountered
    int count = 0;
    
    // Set up column dimensions
    int col_dim1 = col - 3;
    if (col_dim1 < 0) {
        col_dim1 = 0;
    }

    int col_dim2 = col + 3;
    if (col_dim2 >= VIEW_HEIGHT) {
        col_dim2 = VIEW_HEIGHT - 1;
    }

    // Set up row dimensions
    int row_dim1 = row - 3;
    if (row_dim1 < 0) {
        row_dim1 = 0;
    }

    int row_dim2 = row + 3;
    if (row_dim2 >= VIEW_WIDTH) {
        row_dim2 = VIEW_WIDTH - 1;
    }
    
    for(int k = 0; k < actors.size(); k++){
        
        int x = actors[k] -> getX();
        int y = actors[k] -> getY();
        
        // check if there is any thiefbot on same square as a factory, if yes then return false immediately
        if(col < VIEW_HEIGHT && row < VIEW_WIDTH && x == col && y == row){
            if(actors[k] -> isEnemy() && actors[k] -> canBeCollected()){
                // at this point we know we have a thiefbot on same square as factory
                return false;
            }
        }
        
        // check within 3 square region
        if(x >= col_dim1 && x <= col_dim2 && y >= row_dim1 && y <= row_dim2){
            if(actors[k] -> isEnemy() && actors[k] -> canBeCollected()){
                // at this point we know we have a thiefbot in region
                count++;
            }
        }
        
    }
    
    if(count >= 3){
        return false;
    }
    
    // at this point count < 3 and no thiefbot on the same square as factory
    if(prod_type == 'r'){
        // regular thiefbot
        int random = randInt(1, 50);
        if(random == 10){

            RegularThiefBot * thief1 = new RegularThiefBot(this,IID_THIEFBOT,col, row, GraphObject::right);
            actors.push_back(thief1);
            playSound(SOUND_ROBOT_BORN);
        }
    } else if(prod_type == 'm'){
        int random = randInt(1, 50);
        if(random == 6){
            MeanThiefBot * thief2 = new MeanThiefBot(this,IID_MEAN_THIEFBOT,col, row, GraphObject::right);
            actors.push_back(thief2);
            playSound(SOUND_ROBOT_BORN);
        }
    }
    
    return true;
    
}


void StudentWorld::cleanUp(){
    
    // clean up dynamically allocated objects
    
    // get rid of actors in vector in reverse order
    for(int k = actors.size() - 1; k >= 0; k--){
        delete actors[k];
    }
    
    // now clean up pointers in vector
    while (!actors.empty()) {
        actors.erase(actors.begin());
    }
    
    // then get rid of the avatar
    delete avatar;
    
}


// handling the loading of a level
int StudentWorld::loadLevel(){
    
    Level lev(assetPath());
    
    // check which level to load using getLevel()
    int level = getLevel();
    std::ostringstream oss;
    if (level < 10) {
        oss << "level" << std::setw(2) << std::setfill('0') << level << ".txt";
    } else {
        oss << "level" << level << ".txt";
    }
    std::string filename = oss.str();
    
    // load specific level
    Level::LoadResult result = lev.loadLevel(filename);
    
    if (result == Level::load_fail_file_not_found){
        return GWSTATUS_PLAYER_WON;
    } else if (result == Level::load_fail_bad_format){
        return GWSTATUS_LEVEL_ERROR;
    } else if (result == Level::load_success)
    {
        
        // loop to initialize all positions of maze with respective actors, by dynamic allocation and storing pointers into the actors container + avatar pointer
        for(int i = 0; i < VIEW_HEIGHT; i++){
            for(int k = 0; k < VIEW_WIDTH; k++){
                Level::MazeEntry ge = lev.getContentsOf(i,k); // col as first argument, row as second argument
                switch (ge)
                {
                        // remember to add marble,pit,crystal,pea etc....
                    case Level::empty:
                        break;
                    case Level::exit: {
                        Exit * exit = new Exit(this, IID_EXIT, i, k, GraphObject::none);
                        actors.push_back(exit);
                        break;
                    }
                    case Level::player:
                        avatar = new Avatar(this, IID_PLAYER,i,k,GraphObject::right);
                        break;
                    case Level::extra_life: {
                        Extra_Life_Goodie * extra_life = new Extra_Life_Goodie(this, IID_EXTRA_LIFE, i, k, GraphObject::none);
                        actors.push_back(extra_life);
                        break;
                    }
                    case Level::restore_health: {
                        Restore_Health_Goodie * restore_health = new Restore_Health_Goodie(this, IID_RESTORE_HEALTH, i, k, GraphObject::none);
                        actors.push_back(restore_health);
                        break;
                    }
                    case Level::ammo: {
                        Ammo_Goodie * ammo = new Ammo_Goodie(this, IID_AMMO, i, k, GraphObject::none);
                        actors.push_back(ammo);
                        break;
                    }
                    case Level::crystal: {
                        Crystal * crystal = new Crystal(this, IID_CRYSTAL, i, k, GraphObject::none);
                        actors.push_back(crystal);
                        break;
                    }
                    case Level::pit: {
                        Pit * pit = new Pit(this, IID_PIT, i, k, GraphObject::none);
                        actors.push_back(pit);
                        break;
                    }
                    case Level::marble: {
                        Marble * marble = new Marble(this, IID_MARBLE, i, k, GraphObject::none);
                        actors.push_back(marble);
                        break;
                    }
                    case Level::horiz_ragebot: {
                        RageBot * ragebot_h = new RageBot(this, IID_RAGEBOT, i, k, GraphObject::right);
                        actors.push_back(ragebot_h);
                        break;
                    }
                    case Level::vert_ragebot: {
                        RageBot * ragebot_v = new RageBot(this, IID_RAGEBOT, i, k, GraphObject::down);
                        actors.push_back(ragebot_v);
                        break;
                    }
                    case Level::thiefbot_factory: {
                        ThiefBot_Factory * fac1 = new ThiefBot_Factory(this, IID_ROBOT_FACTORY, i, k, GraphObject::none, ThiefBot_Factory::REGULAR);
                        actors.push_back(fac1);
                        break;
                    }
                    case Level::mean_thiefbot_factory: {
                        ThiefBot_Factory * fac2 = new ThiefBot_Factory(this, IID_ROBOT_FACTORY, i, k, GraphObject::none, ThiefBot_Factory::MEAN);
                        actors.push_back(fac2);
                        break;
                    }
                    case Level::wall: {
                        Wall * wall = new Wall(this, IID_WALL, i, k, GraphObject::none);
                        actors.push_back(wall);
                        break;
                    }
                }
            }
        }
        
    }
    
    // function returns 0 if there is no file loading error
    return 0;
}

string StudentWorld::format_text(int score, int level, int lives, int hp, int ammo, int bonus){
    
    std::ostringstream oss;
    oss << "Score: " << std::setw(7) << std::setfill('0') << score
    << "  Level: " << std::setw(2) << std::setfill('0') << level
    << "  Lives: " << std::setw(2) << std::setfill(' ') << lives
    << "  Health: " << std::setw(3) << std::setfill(' ') << hp*5 << "%"
    << "  Ammo: " << std::setw(3) << std::setfill(' ') << ammo
    << "  Bonus: " << std::setw(4) << std::setfill(' ') << bonus;
    return oss.str();
    
}

void StudentWorld::setDisplayText(){
    
    int score = getScore();
    int level = getLevel();
    int game_bonus = bonus; // as bonus is already a data member of StudentWorld
    int livesLeft = getLives();
    
    // Next, creating a string from your statistics, of the form:
    // Score: 0000100 Level: 03 Lives: 3 Health: 70% Ammo: 216 Bonus: 34
    

    string s = format_text(score, level, livesLeft,
                           avatar -> get_hit_points(), avatar -> get_Pea_Count(), game_bonus);
    
    // Finally, update the display text at the top of the screen with your
    // newly created stats
    setGameStatText(s); // calls our provided GameWorld::setGameStatText
}

void StudentWorld::addPea(int startX, int startY, int startDirection){
    Pea * pea = new Pea(this, IID_PEA, startX, startY, startDirection);
    actors.push_back(pea);
}

bool StudentWorld::isClearShot(char dir, int col, int row){
    // check for any (walls, marbles, robots, or robot factories) along the same row
    
    if(dir == 'h'){
        for(int k = 0; k < actors.size(); k++){
            
            int x = actors[k] -> getX();
            int y = actors[k] -> getY();
            
            bool obstruction_position = false;
            if(col > avatar -> getX()){
                if(x > avatar -> getX() && x < col){
                    obstruction_position = true;
                }
            } else if(col < avatar -> getX()){
                if(x > col && x < avatar -> getX()){
                    obstruction_position = true;
                }
            }
            
            // check if there is a robot or marble in matching row
            bool checkp1 = (col < VIEW_HEIGHT && row < VIEW_WIDTH && y == row && obstruction_position && (actors[k] -> hasHealth() == 10 || actors[k] -> isEnemy()));
            
            // check if there is wall or thiefbot factory in matching row
            bool checkp2 = (col < VIEW_HEIGHT && row < VIEW_WIDTH && y == row && obstruction_position && (!actors[k] -> canBeAttacked()) && (!actors[k] -> canMove()) && actors[k] -> isObstruction() && (actors[k] -> hasHealth() == -1) && (!actors[k] -> canBeCollected()));
            
            if(checkp1 || checkp2){
                return false;
            }
            
        }
    } else if(dir == 'v'){
        for(int k = 0; k < actors.size(); k++){
            
            int x = actors[k] -> getX();
            int y = actors[k] -> getY();
            
            // check for obstruction in between robot and avatar
            bool obstruction_position = false;
            if(row > avatar -> getY()){
                if(y > avatar -> getY() && y < row){
                    obstruction_position = true;
                }
            } else if(row < avatar -> getY()){
                if(y > row && y < avatar -> getY()){
                    obstruction_position = true;
                }
            }
            
            // check if there is a robot or marble in matching col
            bool checkp1 = (col < VIEW_HEIGHT && row < VIEW_WIDTH && x == col && obstruction_position && (actors[k] -> hasHealth() == 10 || actors[k] -> isEnemy()));
            
            // check if there is wall or thiefbot factory in matching col
            bool checkp2 = (col < VIEW_HEIGHT && row < VIEW_WIDTH && x == col && obstruction_position && (!actors[k] -> canBeAttacked()) && (!actors[k] -> canMove()) && actors[k] -> isObstruction() && (actors[k] -> hasHealth() == -1) && (!actors[k] -> canBeCollected()));
            
            if(checkp1 || checkp2){
                return false;
            }
            
        }
    }
    
    // at this point no obstructing actors are in the same row or column as the robot
    return true;
}


bool StudentWorld::checkPeaCollision(int step,int col,int row){
    
    if(step == 1){
        
        for(int k = 0; k < actors.size(); k++){
            
            int x = actors[k] -> getX();
            int y = actors[k] -> getY();
            
            if(col < VIEW_HEIGHT && row < VIEW_WIDTH && avatar -> getX() == col && avatar -> getY() == row){
                // then we know that we have an avatar on same square as a pea
                avatar -> damage();
                return true;
            } else if((col < VIEW_HEIGHT && row < VIEW_WIDTH && x == col && y == row && actors[k] -> isEnemy()) || (col < VIEW_HEIGHT && row < VIEW_WIDTH && x == col && y == row && actors[k] -> hasHealth() == 10)){
                // then we know we have either a robot or a marble
                actors[k] -> damage();
                return true;
            }
            
        }
        
        return false;
    } else if(step == 2){
        
        for(int k = 0; k < actors.size(); k++){
            
            int x = actors[k] -> getX();
            int y = actors[k] -> getY();
            
            if(col < VIEW_HEIGHT && row < VIEW_WIDTH && x == col && y == row && (!actors[k] -> canBeAttacked()) && (!actors[k] -> canMove()) && actors[k] -> isObstruction() && (actors[k] -> hasHealth() == -1) && (!actors[k] -> canBeCollected())){
                // then we have collision with a Wall or Thiefbot factory
                actors[k] -> damage();
                return true;
            }
            
        }
        
        return false;
    }
    
    // this means we didn't pass a step which is 1 or 2
    return false;
}

bool StudentWorld::theAvatarCompletedTheCurrentLevel(){
    
    for(int k = 0; k < actors.size(); k++){
        if(actors[k] -> needed_for_exit() && (!(actors[k] -> canBeCollected()))){
            if(actors[k] -> is_actor_alive()){
                return false;
            }
        }
    }
    
    return true;
}

bool StudentWorld::obstructionPresent(int col, int row){
    
    // avatar is obstructing object
    if(col < VIEW_HEIGHT && row < VIEW_WIDTH && avatar -> getX() == col && avatar -> getY() == row){
        return true;
    }
        
    for(int k = 0; k < actors.size(); k++){
        
        int x = actors[k] -> getX();
        int y = actors[k] -> getY();
        
        // check if locations are valid and match
        if(col < VIEW_HEIGHT && row < VIEW_WIDTH && x == col && y == row){
            
            if(actors[k] -> isObstruction()){
                return true;
            }
            
        }
        
    }
    
    return false;
    
}

bool StudentWorld::canBePushed(char dir, int col, int row){
    
    // what is passed in are the coordinates of a marble
    // check adjacent squares of marble depending on direction, to inspect whether they are empty or pits, if they are then return true, for any other actor return false
    
    
    switch (dir) {
        case 'l':
            for(int k = 0; k < actors.size(); k++){
                
                int x = actors[k] -> getX();
                int y = actors[k] -> getY();
                
                if(col - 1 >= 0 && col - 1 < VIEW_HEIGHT && row >= 0 && row < VIEW_WIDTH && x == (col - 1) && y == row){
                    // way of identifying a pit
                    if(actors[k] -> canBeAttacked() && (!actors[k] -> canMove()) && actors[k] -> isObstruction() && (actors[k] -> hasHealth() == -1) && (!actors[k] -> canBeCollected())){
                        return true;
                    } else {
                        // this means that there is some other actor occupying marble adjacent location
                        return false;
                    }
                }
                
            }
            break;
        case 'r':
            for(int k = 0; k < actors.size(); k++){
                
                int x = actors[k] -> getX();
                int y = actors[k] -> getY();
                
                if(col + 1 >= 0 && col + 1 < VIEW_HEIGHT && row >= 0 && row < VIEW_WIDTH && x == (col + 1) && y == row){
                    // way of identifying a pit
                    if(actors[k] -> canBeAttacked() && (!actors[k] -> canMove()) && actors[k] -> isObstruction() && (actors[k] -> hasHealth() == -1) && (!actors[k] -> canBeCollected())){
                        return true;
                    } else {
                        // this means that there is some other actor occupying marble adjacent location
                        return false;
                    }
                }
                
            }
            break;
        case 'u':
            for(int k = 0; k < actors.size(); k++){
                
                int x = actors[k] -> getX();
                int y = actors[k] -> getY();
                
                if(col >= 0 && col < VIEW_HEIGHT && row + 1 >= 0 && row + 1 < VIEW_WIDTH && x == col && y == (row + 1)){
                    // way of identifying a pit
                    if(actors[k] -> canBeAttacked() && (!actors[k] -> canMove()) && actors[k] -> isObstruction() && (actors[k] -> hasHealth() == -1) && (!actors[k] -> canBeCollected())){
                        return true;
                    } else {
                        // this means that there is some other actor occupying marble adjacent location
                        return false;
                    }
                }
                
            }
            break;
        case 'd':
            for(int k = 0; k < actors.size(); k++){
                
                int x = actors[k] -> getX();
                int y = actors[k] -> getY();
                
                if(col >= 0 && col < VIEW_HEIGHT && row - 1 >= 0 && row - 1 < VIEW_WIDTH && x == col && y == (row - 1)){
                    // way of identifying a pit
                    if(actors[k] -> canBeAttacked() && (!actors[k] -> canMove()) && actors[k] -> isObstruction() && (actors[k] -> hasHealth() == -1) && (!actors[k] -> canBeCollected())){
                        return true;
                    } else {
                        // this means that there is some other actor occupying marble adjacent location
                        return false;
                    }
                }
                
            }
            break;
        default:
            // as invalid dir was passed in
            return false;
    }
    
    // if we break that means it has to be an empty spot as no other actor occupies marble adjacent spot so we return true
    return true;
    
}

bool StudentWorld::allCrystalsCollected(){
    
    bool early_break = false;
    
    for(int i = 0; i < actors.size(); i++){
        if(actors[i] -> needed_for_exit() && actors[i] -> canBeCollected()){
            if(actors[i] -> is_actor_alive()){
                early_break = true;
                break;
            }
        }
    }
    
    if(early_break){
        return false;
    } else {
        return true;
    }
}

void StudentWorld::callPushOrSetAliveOnMarble(int dir, int col, int row, char func){
    
    // find the marble's location and then call its push method or set_is_alive method

    for(int k = 0; k < actors.size(); k++){
        
        int x = actors[k] -> getX();
        int y = actors[k] -> getY();
        
        // we may change how we identify a marble later (right now, doing with 10 starting hit points)
        if(col >= 0 && col < VIEW_HEIGHT && row >= 0 && row < VIEW_WIDTH && actors[k] -> hasHealth() == 10 && (!actors[k] -> isEnemy()) && x == col && y == row){
            // call push on the marble we want
            if(func == 'p'){
                actors[k] -> push(dir, col, row);
            } else if(func == 's'){
                actors[k] -> set_is_alive(false);
            }
        
        }
        
    }
    

}

int StudentWorld::move()
{
    
    // update the game status display text here
    setDisplayText();
    
    int current_size = actors.size();
    
    // loop through all actors and make them do something if they are alive
    for(int k = 0; k < current_size; k++){
        int actor_alive = actors[k] -> doSomethingp1();
        if(actor_alive == 0){
            actors[k] -> doSomething();
        }
    }
    
    // notice that we are asking rest of actors to do something before the avatar
    
    // make avatar do something if they are alive
    int avatar_alive = avatar -> doSomethingp1();
    if(avatar_alive == 0){
        avatar -> doSomething();
    }
    
    // check if player died during this phase
    if(!(getAvatar() -> is_actor_alive())){
        return GWSTATUS_PLAYER_DIED;
    }
    
    // check if avatar completed the current level
    if(theAvatarCompletedTheCurrentLevel()){
        
        if(getLevel() == 99){
            return GWSTATUS_PLAYER_WON;
        } else {
            //increase score by bonus as 2000 base points are already handled by exit class
            increaseScore(bonus);
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    
    // remove newly-dead objects after every tick
    for(int i = actors.size() - 1; i >= 0; i--){
        if(!(actors[i] -> is_actor_alive())){
            delete actors[i];
            actors.erase(actors.begin() + i);
        }
    }
    
    // decrement bonus
    reduceLevelBonusByOne();
    
	return GWSTATUS_CONTINUE_GAME;
}




