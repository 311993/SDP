#pragma once

#include "entity.h"
#include "FEHLCD.h"
#include "FEHUtility.h"
#include "list"

using namespace std;

class Game{
    private:
        int tiles[12][128];
        int cameraX, t;
        
        list<Enemy> enemies;
        list<Item> items;
        list<Projectile> projs;
        Entity player;
    public:
        explicit Game();
        int update();
};