#include "entity.h"

Projectile::Projectile(int x, int y, int w, int h, float vx, float vy){

}

Projectile::Projectile(int x, int y, int w, int h){

}

Projectile::Projectile(){

}

void Projectile::update(){

}

//Draw projectile image at x,y coordinates with given x offset - Written by David Stuckey
void Projectile::draw(FEHImage  imgs[], int offset){
    if(vx > 0){
        imgs[4].Draw(x + offset, y);
    }else{
        imgs[5].Draw(x + offset, y);
    }
}