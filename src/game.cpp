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
                
                LCD.SetFontColor(LCD.White);
                if(i%2 == 1){LCD.SetFontColor(LCD.Blue);}
                drawTile(i*20 + cameraX, j*20);

                player.collide(i*20,j*20,20,20);
            }
        }
    }

    //Draw Player
    player.draw(cameraX);

    //Update player
    player.update();

    /*LCD.SetFontColor(LCD.Black);
    LCD.WriteAt("Play Game Here",120, 80);
    LCD.WriteAt("(Touch to return to menu)",20, 104);*/

    if(LCD.KeyState('W')){LCD.WriteLine('W');}
    else if(LCD.KeyState('A')){LCD.WriteLine('A');}
    else if(LCD.KeyState('S')){LCD.WriteLine('S');}
    else if(LCD.KeyState('D')){LCD.WriteLine('D');}    

    scrollScreen();
    printf("%d\n", cameraX);
    t++;
    //int x,y;
    return 0; //LCD.Touch(&x, &y);
}

void Game::drawTile(int x, int y){
    LCD.FillRectangle(x,y,20,20);

    LCD.SetFontColor(LCD.Black);
    
    for(int j = 0; j < 4; j++){
       LCD.DrawHorizontalLine(y + j*5, x, x + 19);  
       
       for(int i = 0; i < 2; i++){
        LCD.DrawVerticalLine(x + i*10 + (j%2)*5, y+j*5, y + j*5 + 4);
       }    
    }
}

void Game::collideTile(){

}

void Game::collideEnemy(){

}

void Game::collideProjectile(){

}

void Game::collideItem(){

}

void Game::cullEntities(){

}

void Game::scrollScreen(){
    cameraX = 160 - player.getX() - 10;

    if(cameraX > 0){ cameraX = 0;}
    if(cameraX < -20*128){cameraX = -20*128;}

    LCD.SetFontColor(LCD.Black);
    LCD.FillRectangle(0,0,20,240);
    LCD.FillRectangle(300,0,20,240);
}

void Game::saveStats(){

}

void Game::displayGameEnd(int condition){

}