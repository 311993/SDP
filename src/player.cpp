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
        vx = -2;
        dir = -1;
    }

    if(LCD.KeyState(TK_RIGHT) && !LCD.KeyState(TK_LEFT)){
        vx = 2;
        dir = 1;
    }

    //If player is falling, disable jump
    if(vy > g){
        jumpAllowed = false;
    }

    //Determine whether player is jumping, if allowed
    if(LCD.KeyState(TK_UP) && jumpAllowed){
        vy = -4;
        jumpAllowed = false;
    }

    //Superclass update - update position/apply gravity
    Entity::update();

    //Win condition
    if(x > 112*20){
        setKill(2);
    }

    //Lose conditions
    if(health <= 0 || y > 240){ 
        kill();
    }

}

//Draw player image at x,y coordinates with given x offset - Written by David Stuckey
void Player::draw(int offset){
    
    //Body
    LCD.SetFontColor(LCD.Blue);
    LCD.FillRectangle(x + offset,y,w,h);

    //Eyes
    LCD.SetFontColor(LCD.White);
    LCD.FillCircle(x + offset + 5 + dir, y + 6, 4);
    LCD.FillCircle(x + offset + 14 + dir, y + 6, 4);

    //Pupils
    LCD.SetFontColor(LCD.Black);
    LCD.FillCircle(x + offset + 5 + 2*dir, y + 6, 2);
    LCD.FillCircle(x + offset + 14 + 2*dir, y + 6, 2);

    //Mouth
    LCD.FillCircle(x + offset + 7 + 2*dir, y + 15, 2);
    LCD.FillCircle(x + offset + 13 + 2*dir, y + 15, 2);
    LCD.FillRectangle(x + offset + 7 + 2*dir, y + 13, 6, 5);

}

//Set killFlag based on win/lose condition - Written by David Stuckey
void Player::setKill(int condition){
    killFlag = condition;
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

//Increase or decrease player score - Written by David Stuckey
void Player::changeScore(int points){
    score += points;
}