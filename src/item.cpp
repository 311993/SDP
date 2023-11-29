#include "entity.h"

//Construct entity with given position, size, and type - Written by David Stuckey
Item::Item(int x, int y, int w, int h, int type) : Entity(x,y,w,h){
    this->type = type;
}

//Construct item with given position and size -default to coin - Written by David Stuckey
Item::Item(int x, int y, int w, int h) : Item(x,y,w,h,0){}

//Construct default item - Written by David Stuckey
Item::Item() : Item(0,0,20,20,0){}

//Override update to do nothing so it doesn't move - Written by David Stuckey
void Item::update(){}

//Draw item image at x,y coordinates with given x offset - Written by David Stuckey
void Item::draw(FEHImage imgs[], int offset){

    switch(type){
        case 0: imgs[7].Draw(x + offset, y); /*Coin*/   break;
        case 1: imgs[8].Draw(x + offset, y); /*Heart*/  break;
        case 2: imgs[9].Draw(x + offset, y); /*Flag*/   break;
    }

}