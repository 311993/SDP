#include "game.h"
#include "math.h"

//Construct game object and initialize game variables - Written by David Stuckey
Game::Game(){
    for(int j = 0; j < 12; j++){
        for(int i = 0; i < 240; i++){
            tiles[j][i] = (j == 11);
        }
    }

    t = 0;
    player = Entity(40, 0, 20, 20);
}

int Game::update(){
    
    //Clear Screen
    LCD.Clear();

    //Draw tiles within screen
    LCD.SetFontColor(LCD.White);
    
     for(int j = 0; j < 12; j++){
        for(int i = 0; i < 16; i++){
            tiles[j][i] > 0 ? LCD.FillRectangle(i*20, j*20, 20, 20): Sleep(0);
        }
    }

    //Update player
    player.update();

    //Draw Player
    player.draw();

    if(LCD.KeyState('W')){LCD.WriteLine('W');}
    else if(LCD.KeyState('A')){LCD.WriteLine('A');}
    else if(LCD.KeyState('S')){LCD.WriteLine('S');}
    else if(LCD.KeyState('D')){LCD.WriteLine('D');}    

    t++;
    return 0;
}

void Game::drawTile(){

}

void Game::collideTile(){

}

void Game::cullEntities(){

}

void Game::scrollScreen(){

}

void Game::saveStats(){

}

void Game::displayGameEnd(int condition){

}