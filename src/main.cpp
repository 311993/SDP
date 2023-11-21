#include "game.h"
#include "iostream"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define BUTTON_HEIGHT 24
#define BUTTON_SEPARATION 8
#define BUTTON_WIDTH 120

int menu();
void stats();
void info();
void credits();
void runGame();

Game game;

using namespace std;

int main(){
    LCD.SetBackgroundColor(LCD.Black);
    LCD.Clear();

    menu();
    game = Game();

    while (1) {
        game.update();
        Sleep(20);
        // Never end
    }
    return 0;
}

int menu(){

    //Loop allows for touch canceling
    while(1){
        //Draw buttons
        for(int i = 0; i < 4; i++){
            LCD.SetFontColor(255);

            LCD.FillRectangle(
                SCREEN_WIDTH/2 - BUTTON_WIDTH/2, 
                SCREEN_HEIGHT +4 -BUTTON_SEPARATION - (4 - i)*(BUTTON_HEIGHT + BUTTON_SEPARATION), 
                BUTTON_WIDTH, 
                BUTTON_HEIGHT
            );
        }

        for(int i = 0; i < 8; i++){
            LCD.SetFontColor(LCD.Red);
            LCD.DrawVerticalLine(i*40 + 20, 0, 240);
            LCD.DrawHorizontalLine(i*40 + 20, 0, 320);
        }

        //Wait for touch + release
        int x,y;
        while(!LCD.Touch(&x, &y)){}
        while(LCD.Touch(&x, &y)){}

        LCD.Clear();

        //Determine which button field touch was in
        if(x > SCREEN_WIDTH/2 - BUTTON_WIDTH/2  && x < SCREEN_WIDTH/2 + BUTTON_WIDTH/2){
            switch(4 - ((SCREEN_HEIGHT - BUTTON_SEPARATION - y)/(BUTTON_HEIGHT + BUTTON_SEPARATION))){
                case 0: runGame();
                break;
                case 1: info();
                break;
                case 2: stats();
                break;
                case 3: credits();
                break;
            }
        }
    }
}

void runGame(){
}

void stats(){

}

void info(){
}

void credits(){
}
