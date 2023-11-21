#include "game.h"
#include "iostream"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define BUTTON_HEIGHT 24
#define BUTTON_SEPARATION 8
#define BUTTON_WIDTH 120

void stats();
void info();
void credits();
void runGame();


using namespace std;

int main(){
    
    const char buttons[][9] = {"Play", "Info", "Stats", "Credits"};

    LCD.SetBackgroundColor(LCD.Black);
    LCD.Clear();

    //Loop allows for touch canceling
    while(1){

        //Draw buttons
        for(int i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++){
            
            int topY = SCREEN_HEIGHT + 4 - BUTTON_SEPARATION - (4 - i)*(BUTTON_HEIGHT + BUTTON_SEPARATION);

            LCD.SetFontColor(LCD.Blue);
            LCD.FillRectangle(SCREEN_WIDTH/2 - BUTTON_WIDTH/2, topY, BUTTON_WIDTH, BUTTON_HEIGHT);

            LCD.SetFontColor(LCD.White);
            LCD.WriteAt(buttons[i], SCREEN_WIDTH/2 - strlen(buttons[i])*6.0, topY + 4);

        }
        
        //Draw temp grid
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

        //Determine which button field the touch was in and run appropriate command
        if(x > SCREEN_WIDTH/2 - BUTTON_WIDTH/2  && x < SCREEN_WIDTH/2 + BUTTON_WIDTH/2){
            switch(3 + (y - SCREEN_HEIGHT + 4 - BUTTON_SEPARATION + BUTTON_HEIGHT/2) / (BUTTON_HEIGHT + BUTTON_SEPARATION)){
                case 0: runGame();  break;
                case 1: info();     break;
                case 2: stats();    break;
                case 3: credits();  break;
            }
        }
    }
}

void runGame(){
    Game game = Game();

    while (1) {
        game.update();
        Sleep(20);
        // Never end
    }
}

void stats(){

}

void info(){
}

void credits(){
}
