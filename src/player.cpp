#include "entity.h"

Player::Player(int x, int y, int w, int h) : Entity(x,y,w,h){
    dir = 1;
}

Player::Player(){

}

void Player::update(){
    vx = 0;
    
    if(LCD.KeyState(TK_LEFT) && !LCD.KeyState(TK_RIGHT)){
        vx = -2;
        dir = -1;
    }

    if(LCD.KeyState(TK_RIGHT) && !LCD.KeyState(TK_LEFT)){
        vx = 2;
        dir = 1;
    }

    if(LCD.KeyState(TK_UP) && jumpAllowed){
        vy = -4;
        jumpAllowed = false;
    }

    Entity::update();
}

void Player::draw(int offset){
    LCD.SetFontColor(LCD.Blue);
    LCD.FillRectangle(x + offset,y,w,h);

    LCD.SetFontColor(LCD.White);
    LCD.FillCircle(x + offset + 5 + dir, y + 6, 4);
    LCD.FillCircle(x + offset + 14 + dir, y + 6, 4);

    LCD.SetFontColor(LCD.Black);
    LCD.FillCircle(x + offset + 5 + 2*dir, y + 6, 2);
    LCD.FillCircle(x + offset + 14 + 2*dir, y + 6, 2);

}

void Player::setKill(int condition){

}

void Player::collide(int x2, int y2, int w2, int h2){
    if(isColliding(x2,y2,w2,h2) && (prevX < x2 + w2 && prevX + w > x2) && prevY <= y2){
        jumpAllowed = true;
    }

    Entity::collide(x2, y2, w2, h2);
}

void Player::healthMinus(){

} 

void Player::healthPlus(){

}

int Player::getScore(){

}

void Player::setScore(){
    
}