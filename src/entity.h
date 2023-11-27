#pragma once
#include "FEHLCD.h"
#include "tigr.h"

//Class representing a game object with position and velocity - Header written by David Stuckey
class Entity{
    protected:
        int x,y,w,h, prevX, prevY, killFlag;
        float vx, vy;
        bool grav;
    public:
        Entity(int x, int y, int w, int h);
        Entity();
        void update();
        void draw(int offset);
        int isKillFlagged();
        void kill();
        bool isColliding(int x2, int y2, int w2, int h2);
        bool isColliding(Entity);
        void collide(int x2, int y2, int w2, int h2);
        int getX(), getY(), getW(), getH();
};

//Class representing a game object with position and velocity 
//and defined movement behavior that damages the player - Header written by David Stuckey
class Enemy : public Entity{
    private:
        int type;
    public:
        Enemy(int x, int y, int w, int h, int type);
        Enemy(int x, int y, int w, int h);
        Enemy();
        void update();
        void draw(int offset);
};

//Class representing a game object with position and velocity 
//that disappears on collision with the player - Header written by David Stuckey
class Item : public Entity{
    public:
        Item(int x, int y, int w, int h);
        Item();
        void update();
        void draw(int offset);
};

//Class representing a game object with position and velocity 
//that disappears when it leaves the screen or on contact with the player or a tile- Header written by David Stuckey
class Projectile : public Entity{
    public:
        Projectile(int x, int y, int w, int h, float vx, float vy);
        Projectile(int x, int y, int w, int h);
        Projectile();
        void update();
        void draw(int offset);
};

//Class representing a game object with position and velocity 
//that can be controlled by the player with keyboard input- Header written by David Stuckey
class Player : public Entity{
    private:
        int health, score, dir;
        bool jumpAllowed;
    public:
        Player(int x, int y, int w, int h);
        Player();
        void update();
        void draw(int offset);
        void setKill(int condition);
        void collide(int x2, int y2, int w2, int h2);
        void healthMinus(), healthPlus();
        int getScore();
        void changeScore(int points);
};