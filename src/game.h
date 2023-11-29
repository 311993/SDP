#pragma once

#include "entity.h"
#include "FEHUtility.h"
#include "stdio.h"
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

        FEHImage assets[16];
    
    public:
        explicit Game();
        ~Game();
        int update();
        void drawTile(int x, int y);
        void collideTile(int x, int y);
        void collideEnemy(Enemy *e);
        void collideProjectile(Projectile *p);
        void collideItem(Item *m);
        void cullEntities();
        void scrollScreen();
        void drawHUD();
        void saveStats();
        int displayGameEnd(int condition);
        static void loadStats(int* dest);
};