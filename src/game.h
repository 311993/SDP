#pragma once

#include "entity.h"
#include "FEHUtility.h"
#include "vector"

using namespace std;

//Class representing main game screen/loop - Header written by David Stuckey
class Game{
    private:
        int tiles[12][128];
        int cameraX, t;
        
        vector<Enemy> enemies;
        vector<Item> items;
        vector<Projectile> projs;
        Player player;
    
    public:
        explicit Game();
        int update();
        void drawTile();
        void collideTile();
        void collideEnemy();
        void collideProjectile();
        void collideItem();
        void cullEntities();
        void scrollScreen();
        void saveStats();
        void displayGameEnd(int condition);
};