#include "FEHLCD.h"
#include "test.h"
#include "FEHUtility.h"
#include "tigr.h"

int main()
{
    int a;
    Test2 t = Test2();
    LCD.WriteLine(t.test());
    while (1) {
        if(LCD.KeyState('W')){LCD.WriteLine('W');}
        else if(LCD.KeyState('A')){LCD.WriteLine('A');}
        else if(LCD.KeyState('S')){LCD.WriteLine('S');}
        else if(LCD.KeyState('D')){LCD.WriteLine('D');}
        
        Sleep(20);
        LCD.Clear();
        // Never end
    }
    return 0;
}