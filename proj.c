#include <stdio.h>
#include <winbase.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <stdfix.h>
#include "game.h"

int main()
{

    system("cls");
    printf("\e[?25l"); // hide cursor

    while (possition != 4)
    {
        refreshCnt();
        option0();
        if (possition == 1)
        {
            option1();
        }

        else if(possition == 2)
        {
            system("cls");
            gotoxy(0 , 0);
            gotoxy(40 , 0);
            for (int i = MatchNumber - 1 ;  i>= 0; i--)
            {
                printf("%s\n" , lastSave[i]);
            }
            int keyPressed3= getch();
            while( keyPressed3 != 13)
            {
                ;
            }
            possition = 1;
        }
        else if (possition == 3)
        {
            /* code */
        }
        printf("p : %d" , possition);
    }
    system("cls");
    return -1;
    // option0();
}
