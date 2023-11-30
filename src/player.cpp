#include "entity.h"

//Create player object at given position and size - Written by David Stuckey
Player::Player(int x, int y, int w, int h) : Entity(x,y,w,h){
    dir = 1;
    health = 3;
    score = 0;
    iframes = 50;
}

//Create default player object - Written by David Stuckey
Player::Player() : Player(150,0,20,20) {}

//Update player position and velocity, take key input for move/jump, check win/lose conditions - Written by David Stuckey
void Player::update(){
    
    //Determine x velocity based on key input
    vx = 0;
    
    if((LCD.KeyState(TK_LEFT) || LCD.KeyState('A'))&& !(LCD.KeyState(TK_RIGHT) || LCD.KeyState('D'))){
        vx = -3;
        dir = -1;
    }

    if((LCD.KeyState(TK_RIGHT) || LCD.KeyState('D'))&& !(LCD.KeyState(TK_LEFT) || LCD.KeyState('A'))){
        vx = 3;
        dir = 1;
    }
    
    //If player is falling, disable jump
    if(vy > 1){
        jumpAllowed = false;
    }

    //Determine whether player is jumping, if allowed
    if((LCD.KeyState(TK_UP) || LCD.KeyState('W') || LCD.KeyState(TK_SPACE)) && jumpAllowed){
        vy = -8;
        jumpAllowed = false;
    }

    //Superclass update - update position/apply gravity
    Entity::update();

    //Decrement invulnerability timer
    if(iframes > -1){
        iframes --;
    }

    //Lose conditions
    if(health <= 0 || y > 240){ 
        kill();
    }

    //Score limits
    if(score > 999){
        score = 999;
    }
    if(score < 0){
        score = 0;
    }

}

//Draw player image at x,y coordinates with given x offset - Written by David Stuckey
void Player::draw(FEHImage  imgs[], int offset){
    
    //Draw player 
    if(dir > 0){
        imgs[0].Draw(x + offset, y);
    }else{
        imgs[1].Draw(x + offset, y);
    }

    //Draw flashing invulnerability aura
    if(iframes %2 == 0){
        LCD.SetFontColor(LCD.Red);
        LCD.DrawRectangle(x + offset,y, 20,20);
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

//Get actual killFlag value - Written by David Stuckey
int Player::getKillFlag(){
    return killFlag;
}

//Decrement player health, force 50 frames between health decreases - Written by David Stuckey
void Player::healthMinus(){
    if(iframes < 0){
        iframes = 50;
        health--;
    }
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