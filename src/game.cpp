#include "game.h"

Game::Game(){
    
}

void Game::update(){
    
    //Clear Screen
    LCD.Clear();
    
    if(LCD.KeyState('W')){LCD.WriteLine('W');}
    else if(LCD.KeyState('A')){LCD.WriteLine('A');}
    else if(LCD.KeyState('S')){LCD.WriteLine('S');}
    else if(LCD.KeyState('D')){LCD.WriteLine('D');}    
}