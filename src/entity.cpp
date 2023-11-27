#include "entity.h"

#define g 0.2

//Construct entity at given x,y position and w,h size - Written by David Stuckey
Entity::Entity(int x, int y, int w, int h) : x{x}, y{y}, w{w}, h{h} {
    vx = 0;
    vy = 0;
}

//Construct default entity - Written by David Stuckey
Entity::Entity() : Entity(0,0,0,0) {}

//Update entity position and velocity - Written by David Stuckey
void Entity::update(){
    prevX = x;
    prevY = y;

    x += vx;
    y += vy;

    vy += g;
}

void Entity::draw(int offset){
    LCD.SetFontColor(LCD.Green);
    LCD.FillRectangle(x + offset,y,w,h);
}

int Entity::isKillFlagged(){
    return 0;
}

void Entity::kill(){
}

bool Entity::isColliding(int x2, int y2, int w2, int h2){
    return (x2 < x + h && x2 + w2 > x && y2 < y + h && y2 + h2 > y);
}

bool Entity::isColliding(Entity){
    return false;
}

void Entity::collide(int x2, int y2, int w2, int h2){
    if(isColliding(x2, y2, w2, h2)){
        if(prevX < x2 + w2 && prevX + w > x2){
            if(prevY <= y2){
                y = y2 - h;
            }else{
                y = y2 + h2;
            }

            vy = 0;
        }

         if(prevY < y2 + h2 && prevY + h > y2){
            if(prevX <= x2){
                x = x2 - w;
            }else{
                x = x2 + w2;
            }

            vx = 0;
        }
    }
}

int Entity::getX(){
    return x;
}

int Entity::getY(){
    return y;
}

int Entity::getW(){
    return w;
}

int Entity::getH(){
    return h;
}