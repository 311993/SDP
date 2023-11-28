#include "entity.h"

//Construct entity at given x,y position and w,h size - Written by David Stuckey
Entity::Entity(int x, int y, int w, int h) : x{x}, y{y}, w{w}, h{h} {
    vx = 0;
    vy = 0;
    grav = 1;
}

//Construct default entity - Written by David Stuckey
Entity::Entity() : Entity(0,0,0,0) {}

//Update entity position and velocity - Written by David Stuckey
void Entity::update(){
    
    //Update position
    prevX = x;
    prevY = y;

    x += vx;
    y += vy;

    //Apply gravitational acceleration if this entity has gravity
    if(grav){
        vy += g;
    }
}

//Draw default entity image at x,y coordinates with a given x offset - Written by David Stuckey
void Entity::draw(FEHImage  imgs[], int offset){
    LCD.SetFontColor(LCD.White);
    LCD.FillRectangle(x + offset,y,w,h);
}

//Get whether the entity is flagged to be culled at frame-end - Written by David Stuckey
int Entity::isKillFlagged(){
    return killFlag;
}

//Flag the entity to be culled at frame-end - Written by David Stuckey
void Entity::kill(){
    killFlag = 1;
}

//Determine if a rectangle intersects the entity - Written by David Stuckey
bool Entity::isColliding(int x2, int y2, int w2, int h2){
    return (x2 < x + h && x2 + w2 > x && y2 < y + h && y2 + h2 > y);
}

//Determine if another entity collides with this one - Written by David Stuckey
bool Entity::isColliding(Entity e){
    return isColliding(e.x, e.y, e.w, e.h);
}

//Push this entity outside of a rectangle if it is intersecting - Written by David Stuckey
void Entity::collide(int x2, int y2, int w2, int h2){
    
    if(isColliding(x2, y2, w2, h2)){
        
        //Vertical correction
        if(prevX < x2 + w2 && prevX + w > x2){
            if(prevY <= y2){
                y = y2 - h;
            }else{
                y = y2 + h2;
            }

            vy = 0;
        }

        //Horizontal correction
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

//Return x position - Written by David Stuckey
int Entity::getX(){
    return x;
}

//Return y position - Written by David Stuckey
int Entity::getY(){
    return y;
}

//Return entity width - Written by David Stuckey
int Entity::getW(){
    return w;
}

//Return entity height - Written by David Stuckey
int Entity::getH(){
    return h;
}