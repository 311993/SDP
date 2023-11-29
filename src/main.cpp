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

Game *gameScreen;

//Run main menu, allow user to enter game, info, stats, or credits screen - Written by David Stuckey
int main(){
    
    //Create game object
    gameScreen = new Game();

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

    //Clear game object
    gameScreen->~Game();
}

//Start a new game, and update game object until game ends - Written by David Stuckey
int runGame(){

    gameScreen->reset();
    //Set initial game background
    LCD.SetBackgroundColor(192 + 256*128 + 256*256*128);
   
    //Run until game update returns a non-zero value (the game ends)
    while (gameScreen->update() == 0) {
        Sleep(20);
    }

    Sleep(20);
}

//Display stats until touch + release - Written by David Stuckey
void stats(){
    
    //Load Stats
    //Format: score 1, score 2, score 3, time 1, time 2, time 3, time last, score last, time 2nd last, score 2nd last
    int stats[10];
    Game::loadStats(stats);

    //Stringify stats
    char score[3][16], time[3][16], last[2][32];

    //high scores + low times
    for(int i = 0; i<3; i++){
        sprintf(score[i], "%d. %03d", i+1, stats[i]);
        sprintf(time[i],  "%d. %03d", i+1, stats[i + 3]);
    }

    //Last two plays
    sprintf(last[0], "Last Played: %03d | %02d:%02d", stats[7], stats[6]/60, stats[6]%60);
    sprintf(last[1], "           : %03d | %02d:%02d", stats[9], stats[8]/60, stats[8]%60);    

    //Display Stats

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
    LCD.WriteAt(time[0], 3*SCREEN_WIDTH/4 - strlen("1. 00:00")*6, 72);
    LCD.WriteAt(time[1], 3*SCREEN_WIDTH/4 - strlen("2. 00:00")*6, 96);
    LCD.WriteAt(time[2], 3*SCREEN_WIDTH/4 -strlen("3. 00:00")*6, 120);

    LCD.WriteAt(score[0], SCREEN_WIDTH/4 - 12 -strlen("1. 000")*6, 72);
    LCD.WriteAt(score[1], SCREEN_WIDTH/4 - 12 -strlen("2. 000")*6, 96);
    LCD.WriteAt(score[2], SCREEN_WIDTH/4 - 12- strlen("3. 000")*6, 120);
    
    //Last two completed games
    LCD.SetFontColor(LCD.Green);
    LCD.WriteAt(last[0], SCREEN_WIDTH/2 - strlen("Last Played: 000 | 00:00")*6, 156);
    LCD.WriteAt(last[1], SCREEN_WIDTH/2 - strlen("           : 000 | 00:00")*6, 180);
    
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
    LCD.WriteAt("How to Play:", 24, 12);
    
    //Write control instructions
    LCD.SetFontColor(LCD.White);
    LCD.WriteAt("* Left/right arrows or", 12, 42);
    LCD.WriteAt("  A/D keys to move", 12, 66);
    LCD.WriteAt("* Up arrow, touch, or ", 12, 90);
    LCD.WriteAt("  spacebar to jump", 12, 114);
    
    //Write gameplay instructions
    LCD.WriteAt("* Avoid ", 12, 138);
    LCD.SetFontColor(LCD.Red);
    LCD.WriteAt("red enemies",12 + strlen("* Avoid ")*12, 138);
    
    LCD.SetFontColor(LCD.White);
    LCD.WriteAt("* Collect ", 12, 162);
    LCD.SetFontColor(LCD.Yellow);
    LCD.WriteAt("yellow coins", 12 + strlen("* Collect ")*12, 162);
    
    LCD.SetFontColor(LCD.White);
    LCD.WriteAt("* Heal with ", 12, 186);
    LCD.SetFontColor(LCD.Green);
    LCD.WriteAt("green hearts", 12 + strlen("* Heal with ")*12, 186);
    
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
    LCD.WriteAt("Cplusplus.com", 24, 180);

    LCD.SetFontColor(LCD.Gray);
    LCD.WriteAt("(Touch to Return to Menu)", SCREEN_WIDTH/2 - strlen("(Touch to Return to Menu)")*6, 216);

    //Wait for touch + release
    int x,y;
    while(!LCD.Touch(&x, &y)){}
    while(LCD.Touch(&x, &y)){}
}