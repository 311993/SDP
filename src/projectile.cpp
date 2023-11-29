#include "entity.h"

//Construct projectile with given position and size - Written by David Stuckey
Projectile::Projectile(int x, int y, int w, int h) : Entity(x,y,w,h){
    created = false;
    grav = false;
}

//Construct default projectile - Written by David Stuckey
Projectile::Projectile() : Projectile(0,0,20,20){}

//One-time velocity set to follow a line towards player position - Written by David Stuckey
void Projectile::create(int px, int py){
    created = true;

    float angle = atan2(py-y,px-x);
    vx = 4*cos(angle);
    vy = 4*sin(angle);
}

//Draw projectile image at x,y coordinates with given x offset - Written by David Stuckey
void Projectile::draw(FEHImage  imgs[], int offset){
    if(vx > 0){
        imgs[4].Draw(x + offset, y);
    }else{
        imgs[5].Draw(x + offset, y);
    }
}

//Get created/pre-created status - Written by David Stuckey
bool Projectile::isCreated(){
    return created;
}