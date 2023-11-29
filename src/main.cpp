#include "game.h"
#include "iostream"
#include "string.h"

//Define size constants
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define CHAR_WIDTH 12
#define CHAR_HEIGHT 24

#define BUTTON_HEIGHT 24
#define BUTTON_SEPARATION 8
#define BUTTON_WIDTH 120

using namespace std;

void stats();
void info();
void credits();
int runGame();

//Run main menu, allow user to enter game, info, stats, or credits screen - Written by David Stuckey
int main(){
    
    //Create game object
    Game gameScreen = Game();

    //Enumerate menu buttons
    const char buttons[][9] = {"Play", "Info", "Stats", "Credits"};

    //Loop allows for touch canceling
    while(1){

        //Clear screen
        LCD.SetBackgroundColor(LCD.Black);
        LCD.Clear();

        //Write Title
        LCD.SetFontColor(LCD.White);
        LCD.WriteAt("SHAPE", SCREEN_WIDTH/2 - strlen("SHAPE")*6, 24);
        LCD.WriteAt("DASH", SCREEN_WIDTH/2 - strlen("DASH")*6, 48);

        //Draw shapes
        //Square
        LCD.SetFontColor(LCD.Blue);
        LCD.FillRectangle(100,30, 20,20);
        
        //Circle
        LCD.SetFontColor(LCD.Red);
        LCD.FillCircle(210, 20, 10);

        //Triangle
        LCD.SetFontColor(LCD.Yellow);
        for(int i = 0; i < 21; i++){
            LCD.DrawLine(210 + i/2, 60 - i, 231 -i/2, 60 -i);
        }

        //Draw buttons
        for(int i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++){
            
            int topY = SCREEN_HEIGHT + 4 - BUTTON_SEPARATION - (4 - i)*(BUTTON_HEIGHT + BUTTON_SEPARATION);

            LCD.SetFontColor(LCD.Blue);
            LCD.FillRectangle(SCREEN_WIDTH/2 - BUTTON_WIDTH/2, topY, BUTTON_WIDTH, BUTTON_HEIGHT);

            LCD.SetFontColor(LCD.White);
            LCD.WriteAt(buttons[i], SCREEN_WIDTH/2 - strlen(buttons[i])*6.0, topY + 4);

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

//Start a new game, and update game object until game ends - Written by David Stuckey
int runGame(){

    Game gameScreen = Game();

    //Set initial game background
    LCD.SetBackgroundColor(192 + 256*128 + 256*256*128);
   
    //Run until game update returns a non-zero value (the game ends)
    while (gameScreen.update() == 0) {
        Sleep(20);
    }

    gameScreen.~Game();

    //Wait for touch release
    int x,y;
    while(LCD.Touch(&x,&y)){}

}

//Display stats until touch + release - Written by David Stuckey
void stats(){
    
    //Clear screen
    LCD.Clear();
    
    //Headers
    LCD.SetFontColor(LCD.Yellow);
    LCD.WriteAt("--Player Records--", SCREEN_WIDTH/2 - strlen("--Player Records--")*6, 12);
    
    LCD.SetFontColor(LCD.Blue);
    LCD.WriteAt("Low Times:", 3*SCREEN_WIDTH/4 - strlen("Low Times:")*6, 48);
    LCD.WriteAt("High Scores:", SCREEN_WIDTH/4 - strlen("High Scores:")*6, 48);
    
    //Records
    LCD.SetFontColor(LCD.White);
    LCD.WriteAt("1. 99:97", 3*SCREEN_WIDTH/4 - strlen("1. 00:00")*6, 72);
    LCD.WriteAt("2. 99:98", 3*SCREEN_WIDTH/4 - strlen("2. 00:00")*6, 96);
    LCD.WriteAt("3: 99:99", 3*SCREEN_WIDTH/4 -strlen("3. 00:00")*6, 120);

    LCD.WriteAt("1. 003", SCREEN_WIDTH/4 - 12 -strlen("1. 000")*6, 72);
    LCD.WriteAt("2. 002", SCREEN_WIDTH/4 - 12 -strlen("2. 000")*6, 96);
    LCD.WriteAt("3: 001", SCREEN_WIDTH/4 - 12- strlen("3. 000")*6, 120);
    
    //Last completed and current (if applicable) game
    LCD.SetFontColor(LCD.Gray);
    LCD.WriteAt("Last Played: 000 | 00:00", SCREEN_WIDTH/2 - strlen("Last Played: 000 | 00:00")*6, 156);
    LCD.SetFontColor(LCD.Green);
    LCD.WriteAt("In Progress: 000 | 00:00", SCREEN_WIDTH/2 - strlen("In Progress: 000 | 00:00")*6, 180);
    
    LCD.SetFontColor(LCD.Gray);
    LCD.WriteAt("(Touch to Return to Menu)", SCREEN_WIDTH/2 - strlen("(Touch to Return to Menu)")*6, 216);
    
    //Wait for touch + release
    int x,y;
    while(!LCD.Touch(&x, &y)){}
    while(LCD.Touch(&x, &y)){}
}

//Display game instructions until touch + release - Written by David Stuckey
void info(){

    //Clear screen
    LCD.Clear();

    //Header
    LCD.SetFontColor(LCD.Blue);
    LCD.WriteAt("How to Play:", SCREEN_WIDTH/2 - strlen("How to Play")*6, 12);
    
    //Write control instructions
    LCD.SetFontColor(LCD.White);
    LCD.WriteAt("* Left/right arrows or", SCREEN_WIDTH/2 - strlen("* Left/right arrows or")*6, 36);
    LCD.WriteAt("  A/D keys to move", SCREEN_WIDTH/2 - strlen("  A/D keys to move")*6, 60);
    LCD.WriteAt("* Up arrow, touch, or ", SCREEN_WIDTH/2 - strlen("* Up arrow, touch, or ")*6, 84);
    LCD.WriteAt("  spacebar to jump", SCREEN_WIDTH/2 - strlen("  spacebar to jump")*6, 108);
    
    //Write gameplay instructions
    LCD.WriteAt("* Avoid ", SCREEN_WIDTH/2 - strlen("* Avoid red enemies")*6, 132);
    LCD.SetFontColor(LCD.Red);
    LCD.WriteAt("red enemies", SCREEN_WIDTH/2 + strlen("* Avoid ")*6 - strlen("red enemies")*6, 132);
    
    LCD.SetFontColor(LCD.White);
    LCD.WriteAt("* Collect ", SCREEN_WIDTH/2 - strlen("* Collect yellow coins")*6, 156);
    LCD.SetFontColor(LCD.Yellow);
    LCD.WriteAt("yellow coins", SCREEN_WIDTH/2 + strlen("* Collect ")*6 - strlen("yellow coins")*6, 156);
    
    LCD.SetFontColor(LCD.White);
    LCD.WriteAt("* Heal with ", SCREEN_WIDTH/2 - strlen("* Heal with green energy")*6, 180);
    LCD.SetFontColor(LCD.Green);
    LCD.WriteAt("green energy", SCREEN_WIDTH/2 + strlen("* Heal with ")*6 - strlen("green energy")*6, 180);
    
    LCD.SetFontColor(LCD.Gray);
    LCD.WriteAt("(Touch to Return to Menu)", SCREEN_WIDTH/2 - strlen("(Touch to Return to Menu)")*6, 216);

    //Wait for touch + release
    int x,y;
    while(!LCD.Touch(&x, &y)){}
    while(LCD.Touch(&x, &y)){}
}

//Display credits until touch + release - Written by David Stuckey
void credits(){
    
    //Clear screen
    LCD.Clear();

    //Display headers
    LCD.SetFontColor(LCD.Yellow);
    LCD.WriteAt("Credits", SCREEN_WIDTH/2 - strlen("Credits")*6, 12);
    LCD.WriteAt("Programmed By:", 12, 48);
    LCD.WriteAt("References:", 12, 132);

    //Display credits
    LCD.SetFontColor(LCD.White);
    LCD.WriteAt("Olivia Smith", 24, 72);
    LCD.WriteAt("David Stuckey",24, 96);
    LCD.WriteAt("FEH Proteus Docs", 24, 156);

    LCD.SetFontColor(LCD.Gray);
    LCD.WriteAt("(Touch to Return to Menu)", SCREEN_WIDTH/2 - strlen("(Touch to Return to Menu)")*6, 216);

    //Wait for touch + release
    int x,y;
    while(!LCD.Touch(&x, &y)){}
    while(LCD.Touch(&x, &y)){}
}