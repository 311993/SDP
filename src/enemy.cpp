#include "entity.h"

//Create enemy object with specified position, size, and type - Written by David Stuckey
Enemy::Enemy(int x, int y, int w, int h, int type) : Entity(x,y,w,h) {
    this->type = type;
    if(type == 0){ dir = -1;}else{grav = 0;}
}

//Create enemy object with specified position and size - defaults to run enemy - Written by David Stuckey
Enemy::Enemy(int x, int y, int w, int h) : Enemy(x,y,w,h,0){}

//Create default enemy object - Written by David Stuckey
Enemy::Enemy() : Enemy(0,0,20,20,0){}

//Update enemy position and velocity - Written by David Stuckey
void Enemy::update(){

    if(type == 0){
        vx = dir*2;
    }

    Entity::update();
}

//Draw enemy image at x,y coordinates with given x offset - Written by David Stuckey
void Enemy::draw(FEHImage  imgs[], int offset){
    switch(type){

        //Running enemy
        case 0:
            if(dir > 0){
                imgs[2].Draw(x + offset, y);
            }else{
                imgs[3].Draw(x + offset, y);
            }
        break;

        //Lava
        case 1:
            imgs[6].Draw(x + offset, y);
        break;
    }
}

//Push enemy out of a rectangle if it is intersecting, bounce horizontally - Written by David Stuckey
void Enemy::collide(int x2, int y2, int w2, int h2){

    //Bounce off blocks
    if(isColliding(x2,y2,w2,h2) && (prevY < y2 + h2 && prevY + h > y2)){
        dir *= -1;
    }

    //Superclass collide - push enemy out of rectangle if intersecting
    Entity::collide(x2, y2, w2, h2);
}