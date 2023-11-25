#pragma once
#include "FEHLCD.h"

class Entity{
    private:
        int x,y,w,h, prevX, prevY;
        float vx, vy;
        bool grav;
    public:
        Entity(int x, int y, int w, int h);
        Entity();
        void update();
        void draw();
};
class Enemy : public Entity{};
class Item : public Entity{};
class Projectile : public Entity{};
class Player : public Entity{};