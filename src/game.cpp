#include "game.h"
#include "math.h"

//Construct game object and initialize game variables - Written by David Stuckey
Game::Game(){

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
                    case 1: tiles[j][i] = 1; break;
                    case 2: enemies.push_back(Enemy(i*20,j*20,20,20,0)); break;
                    case 3: lavas.push_back(Enemy(i*20,j*20,20,20,1)); break;
                    case 4: /*projectile*/ break;
                    case 5: /*coin*/ break;
                    case 6: /*heart*/ break;
                    case 7: /*flag*/ break;
                }
            }
        } 

        //Ensures enemies are drawn behind lava
        enemies.insert(enemies.end(), lavas.begin(), lavas.end());
        
    }catch(int e){
        //Notify user of failed level import
        printf("Missing or unresolvable level file. Error Code: %d", e);

        //Default Level
        for(int j = 0; j < 12; j++){
            for(int i = 0; i < 240; i++){
                tiles[j][i] = 0;
            }
        } 
    }

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

int Game::update(){
    
    //Clear Screen
    LCD.Clear();

    //Draw tiles within screen
    LCD.SetFontColor(LCD.White);
    
     for(int j = 0; j < 12; j++){
        for(int i = cameraX/-20; i < cameraX/-20 + 16; i++){
            if(tiles[j][i]){
                drawTile(i*20 + cameraX, j*20);
                collideTile(i*20, j*20);                
            }
        }
    }

    //For each enemy in the vector, if it is onscreen, draw and update
    for(int i = 0; i < enemies.size(); i++){
        if(enemies.at(i).getX() + cameraX >= 0 && enemies.at(i).getX() + cameraX < 300 && enemies.at(i).getY() <= 240){
            enemies.at(i).draw(assets, cameraX);
            enemies.at(i).update();
        }
    }

    //Draw Player
    player.draw(assets, cameraX);

    //Update player
    player.update();

    scrollScreen();
    drawHUD();
    t++;

    int x,y;
    if(t%3 == 0){
        if(LCD.Touch(&x, &y)){
            saveStats();
            return 1;
        }
    }
    return 0;
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

void Game::collideTile(int x, int y){
    player.collide(x,y,20,20);
    for(int i = 0; i < enemies.size(); i++){
        enemies.at(i).collide(x,y,20,20);
    }
}

void Game::collideEnemy(Enemy e){

}

void Game::collideProjectile(Projectile p){

}

void Game::collideItem(Item m){

}

void Game::cullEntities(){

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

void Game::drawHUD(){
    
    LCD.SetFontColor(LCD.White);

    char tString[8];
    sprintf(tString, "%02d:%02d", t/3000, (t/50)%60);
    LCD.WriteAt(tString,240,4);

    sprintf(tString, "->%03d", player.getScore());
    LCD.WriteAt(tString,144,4);
    
    assets[7].Draw(122,0);

    for(int i = 0; i < player.getHealth(); i++){
        assets[8].Draw(20 + i*20, 0);
    }
}

//Update stats based on player score and time taken - Written by David Stuckey
void Game::saveStats(){
    try{

        //Open stats file
        FILE *data = fopen("data/stats.dat", "r");
        if(data == NULL){throw -1;}

        //Read in old stats
        //Format: score 1, score 2, score 3, time 1, time 2, time 3, time last, score last, time 2nd last, score 2nd last
        int stats[10];

        for(int i = 0; i < 10; i++){
            fscanf(data, "%d", stats + i);
        }

        fclose(data);

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


        //Open data file again, this time to write new stats
        data = fopen("data/stats.dat", "w");
        if(data == NULL){throw -1;}

        //Update stats in file
        for(int i = 0; i < 10; i++){
            fprintf(data, "%d ", stats[i]);
        }

        fclose(data);

    }catch(int e){}
}

int Game::displayGameEnd(int condition){

}