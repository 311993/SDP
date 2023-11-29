#include "entity.h"

//Create player object at given position and size - Written by David Stuckey
Player::Player(int x, int y, int w, int h) : Entity(x,y,w,h){
    dir = 1;
    health = 3;
    score = 0;
}

//Create default player object - Written by David Stuckey
Player::Player() : Player(150,0,20,20) {}

//Update player position and velocity, take key input for move/jump, check win/lose conditions - Written by David Stuckey
void Player::update(){
    
    //Determine x velocity based on key input
    vx = 0;
    
    if(LCD.KeyState(TK_LEFT) && !LCD.KeyState(TK_RIGHT)){
        vx = -3;
        dir = -1;
    }

    if(LCD.KeyState(TK_RIGHT) && !LCD.KeyState(TK_LEFT)){
        vx = 3;
        dir = 1;
    }
    
    //If player is falling, disable jump
    if(vy > 1){
        jumpAllowed = false;
    }

    //Determine whether player is jumping, if allowed
    if(LCD.KeyState(TK_UP) && jumpAllowed){
        vy = -8;
        jumpAllowed = false;
    }

    //Superclass update - update position/apply gravity
    Entity::update();

    //Win condition
    if(x > 113.5*20){
        setKill(2);
    }

    //Lose conditions
    if(health <= 0 || y > 240){ 
        kill();
    }

}

//Draw player image at x,y coordinates with given x offset - Written by David Stuckey
void Player::draw(FEHImage  imgs[], int offset){
    
    if(dir > 0){
        imgs[0].Draw(x + offset, y);
    }else{
        imgs[1].Draw(x + offset, y);
    }
}

//Push player out of a rectangle if it is intersecting, reset jump permissions if applicable - Written by David Stuckey
void Player::collide(int x2, int y2, int w2, int h2){
    
    //Reset jump permissions omly if player is on top of the rectangle
    if(isColliding(x2,y2,w2,h2) && (prevX < x2 + w2 && prevX + w > x2) && prevY <= y2){
        jumpAllowed = true;
    }

    //Superclass collide - push player out of rectangle if intersecting
    Entity::collide(x2, y2, w2, h2);
}

//Set killFlag based on win/lose condition - Written by David Stuckey
void Player::setKill(int condition){
    killFlag = condition;
}

//Decrement player health - Written by David Stuckey
void Player::healthMinus(){
    health--;
} 

//Increment player health - Written by David Stuckey
void Player::healthPlus(){
    health++;
}

//Get player score - Written by David Stuckey
int Player::getScore(){
    return score;
}

//Get player health - Written by David Stuckey
int Player::getHealth(){
    return health;
}

//Increase or decrease player score - Written by David Stuckey
void Player::changeScore(int points){
    score += points;
}