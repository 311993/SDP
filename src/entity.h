#pragma once
#include "FEHLCD.h"

class Entity{
    private:
        int x,y,w,h, prevX, prevY, killFlag;
        float vx, vy;
        bool grav;
    public:
        Entity(int x, int y, int w, int h);
        Entity();
        void update();
        void draw();
        int isKillFlagged();
        void kill();
        bool isColliding(int x, int y, int w, int h);
        bool isColliding(Entity);
        int getX(), getY(), getW(), getH();
};

class Enemy : public Entity{
    private:
        int type;
    public:
        Enemy(int x, int y, int w, int h, int type);
        Enemy(int x, int y, int w, int h);
        Enemy();
        void update();
        void draw();
};

class Item : public Entity{
    public:
        Item(int x, int y, int w, int h);
        Item();
        void update();
        void draw();
};

class Projectile : public Entity{
    public:
        Projectile(int x, int y, int w, int h, float vx, float vy);
        Projectile(int x, int y, int w, int h);
        Projectile();
        void update();
        void draw();
};

class Player : public Entity{
    private:
        int health, score;
    public:
        Player(int x, int y, int w, int h);
        Player();
        void update();
        void draw();
        void setKill(int condition);
        void healthMinus(), healthPlus();
        int getScore();
        void setScore();
};