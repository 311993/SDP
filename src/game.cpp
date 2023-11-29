#include "game.h"
#include "math.h"

//Construct game object and initialize game variables - Written by David Stuckey
Game::Game(){
    try{
        //Load images
        assets[0].Open("assets/playerLFEH.pic");
        assets[1].Open("assets/playerRFEH.pic");
        assets[2].Open("assets/enemyLFEH.pic");
        assets[3].Open("assets/enemyRFEH.pic");
        assets[4].Open("assets/projLFEH.pic");
        assets[5].Open("assets/projRFEH.pic");
        assets[6].Open("assets/lavaFEH.pic");
        assets[7].Open("assets/coinFEH.pic");
        assets[8].Open("assets/heartFEH.pic");
        assets[9].Open("assets/finishFEH.pic");
        
    }catch(int e){
        //Notify user of failed import
        printf("Missing or unresolvable image file. Error Code: %d", e);
    }

    Game::reset();
}

void Game::reset(){
    
    enemies.clear();
    items.clear();
    projs.clear();
    

    try{
        //Open level data file
        FILE *level = fopen("data/level.dat", "r");

        if(level == NULL){throw -1;}
        
        int temp;
        vector<Enemy> lavas;
        
        //Load in tiles from level data
        for(int i = 0; i < 128; i++){
            for(int j = 0; j < 12; j++){
                
                fscanf(level, "%d", &temp);
                tiles[j][i] = 0;

                switch(temp){
                    /*Tile*/    case 1: tiles[j][i] = 1; break;
                    /*Enemy*/   case 2: enemies.push_back(Enemy(i*20,j*20,20,20,0)); break;
                    /*Lava*/    case 3: lavas.push_back(Enemy(i*20,j*20,20,20,1)); break;
                    /*Proj*/    case 4: projs.push_back(Projectile(i*20,j*20,20,20)); break;
                    /*Coin*/    case 5: items.push_back(Item(i*20,j*20,20,20,0)); break;
                    /*Heart*/   case 6: items.push_back(Item(i*20,j*20,20,20,1)); break;
                    /*Flag*/    case 7: items.push_back(Item(i*20,j*20,20,40,2)); break;
                }
            }
        } 
    
        //Ensures enemies are drawn behind lava
        enemies.insert(enemies.end(), lavas.begin(), lavas.end());
        
    }catch(int e){
        //Notify user of failed import
        printf("Missing or unresolvable level file. Error Code: %d", e);

        //Default Level
        for(int j = 0; j < 12; j++){
            for(int i = 0; i < 240; i++){
                tiles[j][i] = 0;
            }
        } 
    }

    t = 0;
    cameraX  = 0;
    player = Player(150, 20, 20, 20);
}

//Explicit destructor to prevent mem leaks with images - Written by David Stuckey
Game::~Game(){
    for(int i = 0; i < 16; i++){
        assets[i].Close();
    }
}

//Update game screen - Written by David Stuckey
int Game::update(){
    
    //Clear Screen
    LCD.Clear();

    //Draw tiles within screen
     for(int j = 0; j < 12; j++){
        for(int i = cameraX/-20; i < cameraX/-20 + 16; i++){
            if(tiles[j][i]){
                drawTile(i*20 + cameraX, j*20);
                collideTile(i*20, j*20);                
            }
        }
    }

    //For each item in the vector, if it is onscreen, draw and update
    for(int i = 0; i < items.size(); i++){
        if(items.at(i).getX() + cameraX >= 0 && items.at(i).getX() + cameraX < 300 && items.at(i).getY() <= 240){
            items.at(i).draw(assets, cameraX);
            items.at(i).update();
            collideItem(&items.at(i));
        }
    }

    //For each enemy in the vector, if it is onscreen, draw and update
    for(int i = 0; i < enemies.size(); i++){
        if(enemies.at(i).getX() + cameraX >= 0 && enemies.at(i).getX() + cameraX < 300 && enemies.at(i).getY() <= 240){
            enemies.at(i).draw(assets, cameraX);
            enemies.at(i).update();
            collideEnemy(&enemies.at(i));
        }
    }

    //For each projectile in the vector, if it is onscreen, draw and update
    for(int i = 0; i < projs.size(); i++){
        if(projs.at(i).getX() + cameraX >= 0 && projs.at(i).getX() + cameraX < 300 && projs.at(i).getY() <= 240 && projs.at(i).getY() >= 0){
            
            //Set projectile velocity when it first enters the screen
            if(!projs.at(i).isCreated()){
                projs.at(i).create(player.getX(), player.getY());
            }

            projs.at(i).draw(assets, cameraX);
            projs.at(i).update();
            collideProjectile(&projs.at(i));

        }else{
            
            //If the projectile leaves the screen, kill-flag it
            if(projs.at(i).isCreated()){
                projs.at(i).kill();
            }
        }
    }

    //Draw Player
    player.draw(assets, cameraX);

    //Update player
    player.update();

    //Remove entities from itme/projectile vectors if they are kill-flagged
    cullEntities();

    //Scroll screen
    scrollScreen();
    
    //Draw HUD
    drawHUD();
    
    //Increment frame timer
    t++;

    //Win/Lose
    int result = player.getKillFlag();
    return result;
}

//Draw tile image at given x and y coordinate - Written by David Stuckey
void Game::drawTile(int x, int y){
    
    //Draw Base with alternating colors
    LCD.SetFontColor(LCD.White);
    if((x - cameraX + y)%40 == 0){LCD.SetFontColor(LCD.Gray);}
    LCD.FillRectangle(x,y,20,20);

    //Draw brick-like grid
    LCD.SetFontColor(LCD.Black);
    
    for(int j = 0; j < 4; j++){
       LCD.DrawHorizontalLine(y + j*5, x, x + 19);  
       
       for(int i = 0; i < 2; i++){
        LCD.DrawVerticalLine(x + i*10 + (j%2)*5, y+j*5, y + j*5 + 4);
       }    
    }
}

//Prevent player and enemies from entering a tile - Written by David Stuckey
void Game::collideTile(int x, int y){
    player.collide(x,y,20,20);
    for(int i = 0; i < enemies.size(); i++){
        enemies.at(i).collide(x,y,20,20);
    }
}

//Damage player if enemy is intersecting - Written by David Stuckey
void Game::collideEnemy(Enemy *e){
    if(player.isColliding(*e)){
        player.healthMinus();
    }
}

//Damage player an kill projectile if intersecting - Written by David Stuckey
void Game::collideProjectile(Projectile *p){
    if(player.isColliding(*p)){
        player.healthMinus();
        p->kill();
    }
}

//Alter player state and remove item if intersecting - Written by David Stuckey
void Game::collideItem(Item *m){
    if(player.isColliding(*m)){
        switch(m->getType()){
            case 1: //Heart
                player.healthPlus();
            case 0: //Coin
                player.changeScore(10);
            break;
            case 2: //Flag
                player.changeScore(100);
                player.setKill(2);
            break;
        }

        m->kill();
    }
}

//Remove kill-flagged items and projectiles - Written by David Stuckey
void Game::cullEntities(){

    //Remove items
    for(int i = 0; i < items.size(); i++){
        if(items.at(i).isKillFlagged()){
            items.erase(items.begin() + i);   
        }
    }

    //Remove projectiles
    for(int i = 0; i < projs.size(); i++){
        if(projs.at(i).isKillFlagged()){
            projs.erase(projs.begin() + i);   
        }
    }
}

//Calculate amount to scroll screen to keep player in center, mask screen edges to hide artifacting - Written by David Stuckey
void Game::scrollScreen(){

    //Calculate screen scrolling to keep player centered
    cameraX = 160 - player.getX() - 10;

    //Limit scroll to prevent drawing beyond limits of tile array
    if(cameraX > 0){ cameraX = 0;}
    if(cameraX < -20*113){cameraX = -20*113;}

    //Mask screen edges to hide scrolling artifacts
    LCD.SetFontColor(LCD.Black);
    LCD.FillRectangle(0,0,20,240);
    LCD.FillRectangle(300,0,20,240);
    LCD.FillRectangle(0,0,320,20);
    LCD.FillRectangle(0,220,320,20);
}

//Draw HUD at top of screen to display health, time, and score - Written by David Stuckey
void Game::drawHUD(){
    
    LCD.SetFontColor(LCD.White);

    //Display time in mm:ss (t is in frames ~ 50th of a second)
    char tString[8];
    sprintf(tString, "%02d:%02d", t/3000, (t/50)%60);
    LCD.WriteAt(tString,240,4);

    //Display score with coin symbol in front
    sprintf(tString, "->%03d", player.getScore());
    LCD.WriteAt(tString,144,4);
    
    assets[7].Draw(122,0);

    //Display as many hearts as the player has
    for(int i = 0; i < player.getHealth(); i++){
        assets[8].Draw(20 + i*20, 0);
    }
}

//Helper function to load stats - Written by David Stuckey
void Game::loadStats(int* dest){

    try{
        //Open stats file
        FILE *data = fopen("data/stats.dat", "r");
        if(data == NULL){throw -1;}

        //Read in stats
        for(int i = 0; i < 10; i++){
            fscanf(data, "%d", dest + i);
        }

        fclose(data);
    }catch(int e){}
}

//Update stats based on player score and time taken - Written by David Stuckey
void Game::saveStats(){

    //Read in old stats
    //Format: score 1, score 2, score 3, time 1, time 2, time 3, time last, score last, time 2nd last, score 2nd last
    int stats[10];
    loadStats(stats);

    //Update high scores
    if(player.getScore() > stats[2]){
        stats[2] = player.getScore();
    }

    if(stats[2] > stats[1]){
        int temp = stats[2];
        stats[2] = stats[1];
        stats[1] = temp;
    }
    
    if(stats[1] > stats[0]){
        int temp = stats[1];
        stats[1] = stats[0];
        stats[0] = temp;
    }

    //Update low times
    if(t/50 < stats[5]){
        stats[5] = t/50;
    }

    if(stats[5] < stats[4]){
        int temp = stats[5];
        stats[5] = stats[4];
        stats[4] = temp;
    }
    
    if(stats[4] < stats[3]){
        int temp = stats[4];
        stats[4] = stats[3];
        stats[3] = temp;
    }

    //Update two previous games in record
    stats[8] = stats[6];
    stats[9] = stats[7];

    stats[6] = t/50;
    stats[7] = (player.getScore() > 999 ? 999 : player.getScore());

    try{
        //Open data file again, this time to write new stats
        FILE *data = fopen("data/stats.dat", "w");
        if(data == NULL){throw -1;}

        //Update stats in file
        for(int i = 0; i < 10; i++){
            fprintf(data, "%d ", stats[i]);
        }

        fclose(data);

    }catch(int e){}
}

int Game::displayGameEnd(int condition){
    int x,y;
    if(condition > 0){
        saveStats();
        while(true){
            if(LCD.Touch(&x, &y)){
                return 1;
            }
        }
    }

    return 0;
}