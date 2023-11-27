#include "game.h"
#include "math.h"

//Construct game object and initialize game variables - Written by David Stuckey
Game::Game(){
    for(int j = 0; j < 12; j++){
        for(int i = 0; i < 240; i++){
            tiles[j][i] = (j == 11);
        }
    }

    tiles[10][4] = 1;
    tiles[9][6] = 1;

    t = 0;
    cameraX  = 0;
    player = Player(150, 0, 20, 20);
}

int Game::update(){
    
    //Clear Screen
    LCD.Clear();

    //Draw tiles within screen
    LCD.SetFontColor(LCD.White);
    
     for(int j = 0; j < 12; j++){
        for(int i = cameraX/-20 + 1; i < cameraX/-20 + 16; i++){
            if(tiles[j][i] > 0){
                drawTile(i*20 + cameraX, j*20);
                collideTile(i*20, j*20);                
            }
        }
    }

    //Draw Player
    player.draw(cameraX);

    //Update player
    player.update();

    scrollScreen();
    t++;
    //int x,y;
    return 0; //LCD.Touch(&x, &y);
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
    if(cameraX < -20*120){cameraX = -20*120;}

    //Mask screen edges to hide scrolling artifacts
    LCD.SetFontColor(LCD.Black);
    LCD.FillRectangle(0,0,20,240);
    LCD.FillRectangle(300,0,20,240);
}

void Game::saveStats(){

}

void Game::displayGameEnd(int condition){

}