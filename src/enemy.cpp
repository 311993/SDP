#include "entity.h"

Enemy::Enemy(int x, int y, int w, int h, int type) : Entity(x,y,w,h) {
    this->type = type;
    if(type == 0){ dir = -1;}
}

Enemy::Enemy(int x, int y, int w, int h) : Enemy(x,y,w,h,0){}

Enemy::Enemy() : Enemy(0,0,20,20,0){}

void Enemy::update(){

    if(type == 0){
        vx = dir*2;
    }

    Entity::update();
}

void Enemy::draw(FEHImage  imgs[], int offset){
    if(dir > 0){
        imgs[2].Draw(x + offset, y);
    }else{
        imgs[3].Draw(x + offset, y);
    }
}

void Enemy::collide(int x2, int y2, int w2, int h2){

    //Bounce off blocks
    if(isColliding(x2,y2,w2,h2) && (prevX < y2 + h2 && prevY + h > y2)){
        dir *= -1;
    }

    //Superclass collide - push enemy out of rectangle if intersecting
    Entity::collide(x2, y2, w2, h2);
}