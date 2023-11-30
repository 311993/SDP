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

//Reset game state to initial values, load in level data - Written by David Stuckey
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
        for(int i = 0; i < 192; i++){
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
            for(int i = 0; i < 192; i++){
                tiles[j][i] = 0;
            }
        } 
    }

    t = 0;
    cameraX  = 0;
    player = Player(180, 20, 20, 20);
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
    int result = displayGameEnd(player.getKillFlag());
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
            break;
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
    if(cameraX < -20*177){cameraX = -20*177;}

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
    LCD.WriteAt(tString,164,4);
    
    assets[7].Draw(142,0);

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

//Display game/win screen and end the game if applicable, returns the condition passed in - Written by David Stuckey
int Game::displayGameEnd(int condition){
    int x,y;
    
    //When condition is non-zero, game should end
    if(condition > 0){
        
        int coins;

        //Win Case
        if(condition == 2){

        //Calculate coins obtained
        coins = player.getScore()/10 - 10;

        //Draw win screen
        LCD.SetBackgroundColor(96);
        LCD.Clear();
        
        //Win display flanked by player and flag
        LCD.SetFontColor(LCD.Green);
        LCD.WriteAt("You Win!", 160 - strlen("You Win!")*6, 24);
        assets[0].Draw(84, 20);
        assets[9].Draw(217, 20);
        
        //Lose Case
        }else{

            //Draw lose screen
            LCD.SetBackgroundColor(96*256*256);
            LCD.Clear();
            
            //Game over display flanked by enemies
            LCD.SetFontColor(LCD.Red);
            LCD.WriteAt("Game Over", 160 - strlen("Game Over")*6, 24);
            assets[2].Draw(84, 20);
            assets[5].Draw(217, 20);

            //Calculate number of coins earned
            coins = player.getScore()/10;
        }

        //Endgame score calculation
        //Each heart is +30 pts
        int healthPts = player.getHealth()*30;

        //Every 3 seconds is -1 pt, to a max penalty of 50
        int timePts = -t/150;
        if(timePts < -50){ timePts = -50;}

        player.changeScore(healthPts + timePts);
        player.update();

        //Display stats
        LCD.SetFontColor(LCD.White);

        //Display finish time in mm:ss (t is in frames ~ 50th of a second)
        //times used are frame-derived to be fair to users with different frame-rates
        char tString[24];
        sprintf(tString, "Finish Time: %02d:%02d", t/3000, (t/50)%60);
        LCD.WriteAt(tString, 160 - strlen(tString)*6,60);
        
        //Display final score
        sprintf(tString, "Final Score: %03d", player.getScore());
        LCD.WriteAt(tString,160 - strlen(tString)*6,96);

        //Display score calculation for "educational value"
        LCD.WriteAt("= 10  +30  +100  -  /6", 160 - strlen("= 10  +30  +100  -  /6")*6 ,132);

        //Insert Item Images into equation
        assets[7].Draw(80,128);
        assets[8].Draw(140,128);
        assets[9].Draw(212,108);

        //Draw Clock into equation
        LCD.SetFontColor(LCD.Black);
        LCD.FillCircle(256, 138, 10);

        LCD.SetFontColor(LCD.White);
        LCD.FillCircle(256, 138, 8);

        LCD.SetFontColor(LCD.Black);
        LCD.DrawLine(256, 138, 256, 130);
        LCD.DrawLine(256, 138, 262, 142);

        //Display number of hearts and coins at game end
        LCD.SetFontColor(LCD.White);
        
        sprintf(tString, "  = %02d           = %d", coins, player.getHealth());
        LCD.WriteAt(tString, 36, 180);
        
        assets[7].Draw(38,176);
        assets[8].Draw(218,176);

        //Tell user how to return to menu
        LCD.SetFontColor(LCD.Gray);
        LCD.WriteAt("(Touch to Return to Menu)", 160 - strlen("(Touch to Return to Menu)")*6, 216);

        //Don't update time records for game overs
        if(condition == 1){
            t = 299999;
        }

        //Update stats    
        saveStats();

        //Wait for touch and release
        while(!LCD.Touch(&x, &y)){}
        while(LCD.Touch(&x, &y)){}

        return condition;
    }

    return 0;
}