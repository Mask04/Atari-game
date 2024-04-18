#include <stdio.h>
#include <winbase.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <unistd.h>
#define spawn_cooldown_mine 2000
#define height 30
#define width 100
#define spawn_cooldown_bomb 9000
#define spawn_cooldown_jang 6700
#define spawn_cooldown_hjet 15000
#define spawn_cooldown_tjet 1300
DWORD lastMineRealeseTime;
DWORD now;
DWORD time0;
DWORD now2;
DWORD lastBombRT;
DWORD now3;
DWORD now4;
DWORD lastJangRT;
DWORD now5;
DWORD lastHjetRT;
DWORD now6;
DWORD lastTJRT;

int HP = 100;
int SCORE = 0;
void gotoxy(int x, int y)
{
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void PrintBorder()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); // yellow

    gotoxy(0, 0);
    for (int i = 0; i < 100; i++)
    {
        printf("-");
    }
    gotoxy(0, 30);
    for (int i = 0; i < 100; i++)
    {
        printf("-");
    }
    printf("\n");
    gotoxy(0, 0);
    for (int i = 0; i <= 30; i++)
    {
        gotoxy(0, i);
        printf("+");
    }
    gotoxy(100, 0);
    for (int i = 0; i <= 30; i++)
    {
        gotoxy(100, i);
        printf("+");
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // white
}

struct jet
{
    int health;
    char shape[6][9];
};
int xj = width / 2 - 5;
int yj = height - 10;
struct jet Myjet;
//     ^
//    | |
//   ||0||
//  /  .  \       
// |--| |--|
//   /|||\

struct fire
{
    int xf;
    int yf;
};

struct fire shoot[1000];

struct mine
{
    char shape[3][5];
    int xm;
    int ym;
};

struct mine mineA[1000];

//  _!_
// (_o_)
//  ***

struct bomb
{
    char shape[3][13];
    int xb;
    int yb;
    int health;
};
struct bomb bombA[100];

//              __
//    _________/F/ //line 1
//   (__________/  //line 2

struct gangande
{
    int health;
    int xg;
    int yg;
    char shape[5][9];
};
struct gangande jang[200];

//                /\\   //\ line 0
//                \  \ /  / 1
//                 \||.||/  2
//                  \\V//   3
//                   \./    4

struct bombPack
{
    int health;
    int xbp;
    int ybp;
    char shape[1][5];
};

struct bombPack bp[100];

// [*b*]     line 0

struct healthJet
{
    int health;
    int xhj;
    int yhj;
    char shape[2][13];
};

struct healthJet hj[100];

//     __|__
// ---o-(_)-o---

struct healthPack
{
    int xhp;
    int yhp;
    char shape[1][5];
};
// [ + ]
struct healthPack hpack[100];

struct tirJang
{
    int xtj;
    int ytj;
    char shape;
};
struct tirJang tJ[1000];
//  |

void tirJangShapeInitialize(struct tirJang *a)
{
    a->shape = '|';
}
void tirJangInitialize()
{
    for (int i = 0; i < 1000; i++)
    {
        tirJangShapeInitialize(&tJ[i]);
    }
}
void printtirJang(struct tirJang a)
{
    gotoxy(a.xtj, a.ytj);
    printf("%c", a.shape);
}
void deletetirJang(struct tirJang a)
{
    gotoxy(a.xtj, a.ytj);
    printf(" ");
}
void healthPackShapeInitialize(struct healthPack *a)
{
    a->shape[0][0] = '[';
    a->shape[0][1] = ' ';
    a->shape[0][2] = '+';
    a->shape[0][3] = ' ';
    a->shape[0][4] = ']';
}

void healthPackInitialize()
{
    for (int i = 0; i < 100; i++)
    {
        healthPackShapeInitialize(&hpack[i]);
    }
}

void printhealthpack(struct healthPack a)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2); // green
    gotoxy(a.xhp, a.yhp);
    for (int i = 0; i < 5; i++)
    {
        printf("%c", a.shape[0][i]);
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // white
}

void deleteHealthPack(struct healthPack a)
{
    for (int j = 0; j < 1; j++)
    {
        gotoxy(a.xhp, a.yhp + j);
        for (int i = 0; i < 5; i++)
        {
            printf(" ");
        }
    }
}

void healthjetShapeInitialize(struct healthJet *a)
{
    a->health = 2;
    // line 0
    a->shape[0][0] = ' ';
    a->shape[0][1] = ' ';
    a->shape[0][2] = ' ';
    a->shape[0][3] = ' ';
    a->shape[0][4] = '_';
    a->shape[0][5] = '_';
    a->shape[0][6] = '|';
    a->shape[0][7] = '_';
    a->shape[0][8] = '_';
    a->shape[0][9] = ' ';
    a->shape[0][10] = ' ';
    a->shape[0][11] = ' ';
    a->shape[0][12] = ' ';
    // line 1
    a->shape[1][0] = '-';
    a->shape[1][1] = '-';
    a->shape[1][2] = '-';
    a->shape[1][3] = 'o';
    a->shape[1][4] = '-';
    a->shape[1][5] = '(';
    a->shape[1][6] = '_';
    a->shape[1][7] = ')';
    a->shape[1][8] = '-';
    a->shape[1][9] = 'o';
    a->shape[1][10] = '-';
    a->shape[1][11] = '-';
    a->shape[1][12] = '-';
}

void healthjetInitialize()
{
    srand(time(NULL));
    for (int i = 0; i < 100; i++)
    {
        healthjetShapeInitialize(&hj[i]);
        hj[i].xhj = rand() % 74 + 8;
        hj[i].yhj = 3;
    }
}

void printHealthjet(struct healthJet a)
{
    for (int j = 0; j < 2; j++)
    {
        gotoxy(a.xhj, a.yhj + j);
        for (int i = 0; i < 13; i++)
        {
            printf("%c", a.shape[j][i]);
        }
    }
}

void deleteHealthjet(struct healthJet a)
{
    for (int j = 0; j < 2; j++)
    {
        gotoxy(a.xhj, a.yhj + j);
        for (int i = 0; i < 13; i++)
        {
            printf(" ");
        }
    }
}

void bombPackShapeInitialize(struct bombPack *a)
{
    a->health = 1;
    a->shape[0][0] = '[';
    a->shape[0][1] = '*';
    a->shape[0][2] = 'b';
    a->shape[0][3] = '*';
    a->shape[0][4] = ']';
}

void bombPackInitialize()
{
    for (int i = 0; i < 100; i++)
    {
        bombPackShapeInitialize(&bp[i]);
    }
}

void printBombPack(struct bombPack a)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 192); //
    for (int j = 0; j < 1; j++)
    {
        gotoxy(a.xbp, a.ybp + j);
        for (int i = 0; i < 5; i++)
        {
            printf("%c", a.shape[j][i]);
        }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); //
}

void deleteBombPack(struct bombPack a)
{
    for (int j = 0; j < 1; j++)
    {
        gotoxy(a.xbp, a.ybp + j);
        for (int i = 0; i < 5; i++)
        {
            printf(" ");
        }
    }
}

void jangShapeInitialize(struct gangande *a)
{
    a->health = 2;
    // line 0
    a->shape[0][0] = '/';
    a->shape[0][1] = '\\';
    a->shape[0][2] = '\\';
    a->shape[0][3] = ' ';
    a->shape[0][4] = ' ';
    a->shape[0][5] = ' ';
    a->shape[0][6] = '/';
    a->shape[0][7] = '/';
    a->shape[0][8] = '\\';
    // line 1
    a->shape[1][0] = '\\';
    a->shape[1][1] = ' ';
    a->shape[1][2] = ' ';
    a->shape[1][3] = '\\';
    a->shape[1][4] = ' ';
    a->shape[1][5] = '/';
    a->shape[1][6] = ' ';
    a->shape[1][7] = ' ';
    a->shape[1][8] = '/';
    // line 2
    a->shape[2][0] = ' ';
    a->shape[2][1] = '\\';
    a->shape[2][2] = '|';
    a->shape[2][3] = '|';
    a->shape[2][4] = '.';
    a->shape[2][5] = '|';
    a->shape[2][6] = '|';
    a->shape[2][7] = '/';
    a->shape[2][8] = ' ';
    // line 3
    a->shape[3][0] = ' ';
    a->shape[3][1] = ' ';
    a->shape[3][2] = '\\';
    a->shape[3][3] = '\\';
    a->shape[3][4] = 'V';
    a->shape[3][5] = '/';
    a->shape[3][6] = '/';
    a->shape[3][7] = ' ';
    a->shape[3][8] = ' ';
    // line 4
    a->shape[4][0] = ' ';
    a->shape[4][1] = ' ';
    a->shape[4][2] = ' ';
    a->shape[4][3] = '\\';
    a->shape[4][4] = '.';
    a->shape[4][5] = '/';
    a->shape[4][6] = ' ';
    a->shape[4][7] = ' ';
    a->shape[4][8] = ' ';
}

void jangInitialize()
{
    srand(time(NULL));

    for (int i = 0; i < 200; i++)
    {
        jangShapeInitialize(&jang[i]);
        jang[i].xg = rand() % 75 + 10;
        jang[i].yg = 2;
    }
}

void printjang(struct gangande a)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9); //

    for (int j = 0; j < 5; j++)
    {
        gotoxy(a.xg, a.yg + j);
        for (int i = 0; i < 9; i++)
        {
            printf("%c", a.shape[j][i]);
        }
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // w
}

void deletejang(struct gangande a)
{

    for (int j = 0; j < 5; j++)
    {
        gotoxy(a.xg, a.yg + j);
        for (int i = 0; i < 9; i++)
        {
            printf(" ");
        }
    }
}

void bombAfkanShapeInitialize(struct bomb *a)
{
    // a->health 
    // line 0
    a->shape[0][0] = ' ';
    a->shape[0][1] = ' ';
    a->shape[0][2] = ' ';
    a->shape[0][3] = ' ';
    a->shape[0][4] = ' ';
    a->shape[0][5] = ' ';
    a->shape[0][6] = ' ';
    a->shape[0][7] = ' ';
    a->shape[0][8] = ' ';
    a->shape[0][9] = ' ';
    a->shape[0][10] = ' ';
    a->shape[0][11] = '_';
    a->shape[0][12] = '_';
    // line 1
    a->shape[1][0] = ' ';
    a->shape[1][1] = '_';
    a->shape[1][2] = '_';
    a->shape[1][3] = '_';
    a->shape[1][4] = '_';
    a->shape[1][5] = '_';
    a->shape[1][6] = '_';
    a->shape[1][7] = '_';
    a->shape[1][8] = '_';
    a->shape[1][9] = '_';
    a->shape[1][10] = '/';
    a->shape[1][11] = 'F';
    a->shape[1][12] = '/';
    // line 2
    a->shape[2][0] = '(';
    a->shape[2][1] = '_';
    a->shape[2][2] = '_';
    a->shape[2][3] = '_';
    a->shape[2][4] = '_';
    a->shape[2][5] = '_';
    a->shape[2][6] = '_';
    a->shape[2][7] = '_';
    a->shape[2][8] = '_';
    a->shape[2][9] = '_';
    a->shape[2][10] = '_';
    a->shape[2][11] = '/';
    a->shape[2][12] = ' ';
}

void bombInitialize()
{
    for (int i = 0; i < 100; i++)
    {
        bombAfkanShapeInitialize(&bombA[i]);
        bombA[i].xb = 87;
        bombA[i].yb = 2;
    }
}

void printBombA(struct bomb a)
{

    for (int j = 0; j < 3; j++)
    {
        gotoxy(a.xb, a.yb + j);
        for (int i = 0; i < 13; i++)
        {
            printf("%c", a.shape[j][i]);
        }
    }
}
void deletebomb(struct bomb a)
{
    for (int j = 0; j < 3; j++)
    {
        gotoxy(a.xb, a.yb + j);
        for (int i = 0; i < 13; i++)
        {
            printf(" ");
        }
    }
}

void JetShapeInitialize(struct jet *a)
{
    a->health = 100;

    // line 1
    a->shape[0][0] = ' ';
    a->shape[0][1] = ' ';
    a->shape[0][2] = ' ';
    a->shape[0][3] = ' ';
    a->shape[0][4] = '^';
    a->shape[0][5] = ' ';
    a->shape[0][6] = ' ';
    a->shape[0][7] = ' ';
    a->shape[0][8] = ' ';
    // line 2
    a->shape[1][0] = ' ';
    a->shape[1][1] = ' ';
    a->shape[1][2] = ' ';
    a->shape[1][3] = '|';
    a->shape[1][4] = ' ';
    a->shape[1][5] = '|';
    a->shape[1][6] = ' ';
    a->shape[1][7] = ' ';
    a->shape[1][8] = ' ';
    // line 3
    a->shape[2][0] = ' ';
    a->shape[2][1] = ' ';
    a->shape[2][2] = '|';
    a->shape[2][3] = '|';
    a->shape[2][4] = '0';
    a->shape[2][5] = '|';
    a->shape[2][6] = '|';
    a->shape[2][7] = ' ';
    a->shape[2][8] = ' ';
    // line 4
    a->shape[3][0] = ' ';
    a->shape[3][1] = '/';
    a->shape[3][2] = ' ';
    a->shape[3][3] = ' ';
    a->shape[3][4] = '.';
    a->shape[3][5] = ' ';
    a->shape[3][6] = ' ';
    a->shape[3][7] = '\\';
    a->shape[3][8] = ' ';
    // line 5
    a->shape[4][0] = '|';
    a->shape[4][1] = '-';
    a->shape[4][2] = '-';
    a->shape[4][3] = '|';
    a->shape[4][4] = ' ';
    a->shape[4][5] = '|';
    a->shape[4][6] = '-';
    a->shape[4][7] = '-';
    a->shape[4][8] = '|';
    // line 6
    a->shape[5][0] = ' ';
    a->shape[5][1] = ' ';
    a->shape[5][2] = '/';
    a->shape[5][3] = '|';
    a->shape[5][4] = '|';
    a->shape[5][5] = '|';
    a->shape[5][6] = '\\';
    a->shape[5][7] = ' ';
    a->shape[5][8] = ' ';
}
void PrintMyjet()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8); // gray
    JetShapeInitialize(&Myjet);
    // gotoxy(width / 2 - 5 , height - 10);
    for (int j = 0; j < 6; j++)
    {
        gotoxy(xj, yj + j);
        for (int i = 0; i < 9; i++)
        {
            printf("%c", Myjet.shape[j][i]);
        }
    }
    // gotoxy(0 , 31);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // white
}
void deletejet(int x, int y)
{
    // gotoxy(width / 2 - 5 , height - 10);
    for (int j = 0; j < 6; j++)
    {
        gotoxy(x, y + j);
        for (int i = 0; i < 9; i++)
        {
            printf(" ");
        }
    }
}
void printShoot(struct fire a)
{
    gotoxy(a.xf, a.yf);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4); // red
    printf("*");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // white
}
void deleteshoot(struct fire a)
{
    gotoxy(a.xf, a.yf);
    printf(" ");
}

void MineShapeInitialize(struct mine *a)
{

    // line 1
    a->shape[0][0] = ' ';
    a->shape[0][1] = '_';
    a->shape[0][2] = '!';
    a->shape[0][3] = '_';
    a->shape[0][4] = ' ';
    // line 2
    a->shape[1][0] = '(';
    a->shape[1][1] = '_';
    a->shape[1][2] = 'o';
    a->shape[1][3] = '_';
    a->shape[1][4] = ')';
    // line 3
    a->shape[2][0] = ' ';
    a->shape[2][1] = '*';
    a->shape[2][2] = '*';
    a->shape[2][3] = '*';
    a->shape[2][4] = ' ';
}

void mineInitialize()
{
    for (int i = 0; i < 1000; i++)
    {
        MineShapeInitialize(&mineA[i]);
    }
}
void printMine(struct mine a)
{

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5); // purple
    for (int j = 0; j < 3; j++)
    {
        gotoxy(a.xm, a.ym + j);
        for (int i = 0; i < 5; i++)
        {
            printf("%c", a.shape[j][i]);
        }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // white;
}

void deleteMine(struct mine a)
{
    for (int j = 0; j < 3; j++)
    {
        gotoxy(a.xm, a.ym + j);
        for (int i = 0; i < 5; i++)
        {
            printf(" ");
        }
    }
}

int iBefore;
int ShootCnt;
void move(char keyPressed, int *x, int *y)
{
    if (keyPressed == 'w' && *y >= 8)
    {
        *y -= 4;
    }
    if (keyPressed == 's' && *y <= 20)
    {
        *y += 4;
    }
    if (keyPressed == 'd' && *x <= 86)
    {
        *x += 5;
    }
    if (keyPressed == 'a' && *x >= 6)
    {
        *x -= 5;
    }
    static int i = 0;
    iBefore = i;
    if (keyPressed == ' ')
    {
        ShootCnt++;
        shoot[i].xf = *x + 4;
        shoot[i].yf = *y - 1;
        i++;
    }
}
int h;
void shootRefresh()
{
    for (int i = h; i < iBefore; i++)
    {
        if (shoot[i].yf > 2)
        {
            deleteshoot(shoot[i]);
            shoot[i].yf--;
            printShoot(shoot[i]);
        }
        else if (shoot[i].yf == 2)
        {

            deleteshoot(shoot[i]);
            h++;
        }
    }
    // Sleep(1);
}

void setMineY()
{
    for (int i = 0; i < 1000; i++)
    {
        mineA[i].ym = 2;
    }
}
int n;
int minCnt = 0;
void mineRefresh()
{
    now = GetTickCount();
    if (now - lastMineRealeseTime > spawn_cooldown_mine)
    {
        lastMineRealeseTime = now;
        minCnt++;
    }

    for (int i = n; i < minCnt; i++)
    {
        if (mineA[i].ym < 26)
        {
            deleteMine(mineA[i]);
            mineA[i].ym += 2;
            printMine(mineA[i]);
        }
        else if (mineA[i].ym >= 26)
        {
            deleteMine(mineA[i]);
            n++;
            continue;
        }

        // barresi barkhord
        for (int i = n; i < 1000; i++)
        {
            if ((mineA[i].ym + 2 >= yj && mineA[i].ym <= yj + 5) && (mineA[i].xm + 4 >= xj && mineA[i].xm <= xj + 8))
            {
                HP -= 15;
                deleteMine(mineA[i]);
                PrintMyjet();
                n++;
            }
        }
    }

    Sleep(40);
}

void scoreRefresh()
{
    now2 = GetTickCount();

    if (now2 - time0 > 1000)
    {
        SCORE += 10;
        time0 = now2;
    }
}
int bomberCnt = 0;
int q = 0;
int bombCnt = 0;
int c = 0;
void bombRefresh()
{
    now3 = GetTickCount();
    if (now3 - lastBombRT > spawn_cooldown_bomb)
    {
        lastBombRT = now3;
        bomberCnt++;
    }
    static int tmp = 0;
    if (tmp % 20 == 0)
    {

        bp[bombCnt].xbp = bombA[bomberCnt - 1].xb + 5;
        bp[bombCnt].ybp = bombA[bomberCnt - 1].yb + 3;
        bombCnt++;
    }
    for (int i = q; i < bomberCnt; i++)
    {
        if (bombA[i].yb < 26 && bombA[i].xb > 8)
        {
            deletebomb(bombA[i]);
            bombA[i].yb += 2;
            bombA[i].xb -= 7;
            printBombA(bombA[i]);
        }
        else if (bombA[i].yb >= 26 || bombA[i].xb <= 4)
        {
            deletebomb(bombA[i]);
            q++;
            continue;
        }

        // barresi barkhord
        if ((bombA[i].yb >= yj && bombA[i].yb <= yj + 5) && (bombA[i].xb + 12 >= xj && bombA[i].xb <= xj + 8))
        {
            HP -= 20;
            deletebomb(bombA[i]);
            PrintMyjet();
            q++;
        }
    }
    tmp++;
    Sleep(20);
}
void bombPackRefresh()
{
    srand(time(NULL));
    for (int i = c; i < bombCnt; i++)
    {
        if (bp[i].ybp < 26)
        {
            deleteBombPack(bp[i]);
            bp[i].ybp += 2;
            printBombPack(bp[i]);
        }
        else if (bp[i].ybp >= 26)
        {
            deleteBombPack(bp[i]);
            c++;
            continue;
        }

        if ((bp[i].ybp >= yj && bp[i].ybp <= yj + 5) && (bp[i].xbp + 4 >= xj && bp[i].xbp <= xj + 8))
        {
            HP -= 30;
            deleteBombPack(bp[i]);
            PrintMyjet();
            c++;
        }
    }

    Sleep(20);
}
int tirJangCnt;
int b;
void tirJangRefresh()
{

    for (int i = b; i < tirJangCnt; i++)
    {
        if (tJ[i].ytj < 24)
        {
            deletetirJang(tJ[i]);
            tJ[i].ytj += 5;
            printtirJang(tJ[i]);
        }
        else if (tJ[i].ytj >= 24)
        {
            deletetirJang(tJ[i]);
            b++;
            continue;
        }
        // barresi barkhord

        if ((tJ[i].ytj >= yj && tJ[i].ytj <= yj + 5) && (tJ[i].xtj >= xj && tJ[i].xtj <= xj + 8))
        {
            HP -= 10;
            deletetirJang(tJ[i]);
            PrintMyjet();
            b++;
        }
    }

    Sleep(40);
}

int jangandeCnt;
int l;
void jangRefresh()
{
    now4 = GetTickCount();
    if (now4 - lastJangRT > spawn_cooldown_jang)
    {
        lastJangRT = now4;
        jangandeCnt++;
    }

    for (int i = l; i < jangandeCnt; i++)
    {
        now6 = GetTickCount();
        if (now6 - lastTJRT > spawn_cooldown_tjet)
        {
            lastTJRT = now6;
            tJ[tirJangCnt].xtj = jang[i].xg + 4;
            tJ[tirJangCnt].ytj = jang[i].yg + 5;
            tirJangCnt++;
        }
        if (jang[i].yg < 23)
        {
            deletejang(jang[i]);
            jang[i].yg += 2;
            printjang(jang[i]);
        }
        else if (jang[i].yg >= 23)
        {
            deletejang(jang[i]);
            l++;
            continue;
        }
        // barresi barkhord

        if ((jang[i].yg + 4 >= yj && jang[i].yg <= yj + 5) && (jang[i].xg + 8 >= xj && jang[i].xg <= xj + 8))
        {
            HP -= 40;
            deletejang(jang[i]);
            PrintMyjet();
            l++;
        }
    }

    Sleep(55);
}

int p;
int healthPackCnt;
void healthPackRefresh()
{
    for (int i = p; i < healthPackCnt; i++)
    {
        if (hpack[i].yhp < 27)
        {
            deleteHealthPack(hpack[i]);
            hpack[i].yhp += 2;
            printhealthpack(hpack[i]);
        }
        else if (hpack[i].yhp >= 27)
        {
            deleteHealthPack(hpack[i]);
            p++;
            continue;
        }
        // barresi barkhord

        if ((hpack[i].yhp + 1 >= yj && hpack[i].yhp <= yj + 5) && (hpack[i].xhp + 4 >= xj && hpack[i].xhp <= xj + 8))
        {
            HP += 50;
            deleteHealthPack(hpack[i]);
            PrintMyjet();
            p++;
        }
    }

    Sleep(50);
}

int healthJetCnt;
int t;
void healthJetRefresh()
{
    now5 = GetTickCount();
    if (now5 - lastHjetRT > spawn_cooldown_hjet)
    {
        lastHjetRT = now5;
        healthJetCnt++;
    }

    for (int i = t; i < healthJetCnt; i++)
    {
        if (hj[i].yhj < 27)
        {
            deleteHealthjet(hj[i]);
            hj[i].yhj++;
            printHealthjet(hj[i]);
        }
        else if (hj[i].yhj >= 27)
        {
            if (hj[i].yhj == 28)
                continue;
            deleteHealthjet(hj[i]);
            t++;
            continue;
        }
        // barresi barkhord

        if ((hj[i].yhj + 1 >= yj && hj[i].yhj <= yj + 5) && (hj[i].xhj + 12 >= xj && hj[i].xhj <= xj + 8))
        {
            HP -= 12;
            deleteHealthjet(hj[i]);
            PrintMyjet();
            hj[i].yhj = 28;
            t++;
        }
    }

    Sleep(50);
}

void checkshoot()
{
    // minShot
    for (int j = n; j < minCnt; j++)
    {
        for (int i = h; i < ShootCnt; i++)
        {
            if ((shoot[i].xf >= mineA[j].xm && shoot[i].xf <= mineA[j].xm + 4) && (shoot[i].yf >= mineA[j].ym && shoot[i].yf <= mineA[j].ym + 2))
            {
    
                deleteMine(mineA[j]);
                deleteshoot(shoot[i]);
                PrintMyjet();
                mineA[j].ym = 26;
                shoot[i].yf = 1;
                SCORE += 15;
            }
        }
    }
    // bomberShot
    for (int j = q; j < bomberCnt; j++)
    {
        for (int i = h; i < ShootCnt; i++)
        {
            if ((shoot[i].xf >= bombA[j].xb && shoot[i].xf <= bombA[j].xb + 12) && (shoot[i].yf >= bombA[j].yb && shoot[i].yf <= bombA[j].yb + 2))
            {
                deleteshoot(shoot[i]);
                deletebomb(bombA[j]);
                PrintMyjet();
                bombA[j].yb = 26;
                shoot[i].yf = 1;
                SCORE += 18;
            }
        }
    }
    // jangandeShot
    for (int j = l; j < jangandeCnt; j++)
    {
        for (int i = h; i < ShootCnt; i++)
        {
            if ((shoot[i].xf >= jang[j].xg && shoot[i].xf <= jang[j].xg + 8) && (shoot[i].yf >= jang[j].yg && shoot[i].yf <= jang[j].yg + 4))
            {
                deleteshoot(shoot[i]);
                jang[i].health--;
                if (jang[i].health <= 0)
                {
                    deletejang(jang[j]);
                    jang[j].yg = 24;
                    shoot[i].yf = 1;
                    PrintMyjet();
                }
                
                SCORE += 20;
            }
        }
    }
    // healthjetShot
    for (int j = t; j < healthJetCnt; j++)
    {
        for (int i = h; i < ShootCnt; i++)
        {
            if ((shoot[i].xf >= hj[j].xhj && shoot[i].xf <= hj[j].xhj + 12) && (shoot[i].yf >= hj[j].yhj && shoot[i].yf <= hj[j].yhj + 1))
            {
                deleteHealthjet(hj[j]);
                deleteshoot(shoot[i]);
                hpack[healthPackCnt].xhp = hj[j].xhj + 6;
                hpack[healthPackCnt].yhp = hj[j].yhj + 2;
                printhealthpack(hpack[healthPackCnt]);
                healthPackCnt++;
                PrintMyjet();
                hj[j].yhj = 27;
                shoot[i].yf = 1;
            }
        }
    }
}
void printHp()
{
    gotoxy(0 , 31);
    printf("          ");
    gotoxy(0 , 31);
    printf("HP = %d" , HP);
}
void refreshScreen()
{
    while (!kbhit())
    {
        printHp();
        scoreRefresh();
        gotoxy(20, 31);
        printf("SCORE = %d", SCORE);
        PrintMyjet();
        shootRefresh();
        mineRefresh();
        checkshoot();
        bombRefresh();
        bombPackRefresh();
        jangRefresh();
        healthJetRefresh();
        healthPackRefresh();
        tirJangRefresh();
    }
}

int MatchNumber;
char lastSave[25][30];
void saveData()
{
    time_t currentTime;
    time(&currentTime);
    struct tm* timeInfo = localtime(&currentTime);
    sprintf(lastSave[MatchNumber], "TIME : %02d:%02d:%02d SCORE : %d",timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec, SCORE);
}
void saveDatainFile()
{
    FILE *fp;
    fp = fopen("C:\\Users\\ASUS\\OneDrive\\Desktop\\f\\info.txt", "a");
    for (int i = MatchNumber; i >= 0; i--)
    {
        fprintf(fp, "%s\n",lastSave[i]);
    }
    fclose(fp);
}

void refreshCnt()
{
    iBefore = 0;
    ShootCnt = 0;
    h = 0;
    n = 0;
    minCnt = 0;
    bomberCnt = 0;
    q = 0;
    bombCnt = 0;
    c = 0;
    tirJangCnt = 0;
    b = 0;
    jangandeCnt = 0;
    l = 0;
    p = 0;
    healthPackCnt = 0;
    healthJetCnt = 0;
    t = 0;
    SCORE = 0;
    HP = 100;
    xj = width / 2 - 5;
    yj = height - 10; 
}

int possition;
int possition2;
void arrowHere(int realPosition, int arrowPosition)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3); // change color to blue
    if (realPosition == arrowPosition)
        printf(">> ");
    else
    {
        printf("   ");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // reset color to white
    }
}
void option0()
{
    int keyPressed = 0;
    while (keyPressed != 13) // ascii enter key = 13
    {
        system("cls");
        gotoxy(width / 2, height / 2);
        arrowHere(1, possition);

        if (possition == 1)
        {
            printf("    P   L   A   Y\n");
        }
        else
        {
            gotoxy(width / 2, height / 2);
            printf("PLAY\n");
        }

        gotoxy(width / 2, height / 2 + 1);
        arrowHere(2, possition);
        if (possition == 2)
        {
            printf("    S   C   O   R   E   S\n");
        }
        else
        {
            gotoxy(width / 2, height / 2 + 1);
            printf("SCORES\n");
        }
        gotoxy(width / 2, height / 2 + 2);
        arrowHere(3, possition);
        if (possition == 3)
        {
            printf("    D   I   F   F   I   C   U   L   T   Y\n");
        }
        else
        {
            gotoxy(width / 2, height / 2 + 2);
            printf("DIFFICULTY\n");
        }
        gotoxy(width / 2, height / 2 + 3);
        arrowHere(4, possition);
        if (possition == 4)
        {
            printf("    Q   U   I   T\n");
        }
        else
        {
            gotoxy(width / 2, height / 2 + 3);
            printf("QUIT\n");
        }

        keyPressed = getch();
        if ((keyPressed == 80) && (possition != 4)) // ascii 80 =  Down arrow
            possition++;
        else if ((keyPressed == 72) && (possition != 1)) // ascii 72 = up arrow
            possition--;
        else
            possition = possition;
    }
}
void option1()
{

    system("cls");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32); // change color to red
    gotoxy(width / 2, height / 2);
    printf("PRESS ANY BUTTON TO START\n");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // reset color to white
    getch();
    system("cls");
    PrintBorder();
    PrintMyjet();
    mineInitialize();
    bombInitialize();
    bombPackInitialize();
    setMineY();
    jangInitialize();
    healthjetInitialize();
    healthPackInitialize();
    tirJangInitialize();

    for (int i = 0; i < 1000; i++)
    {
        mineA[i].xm = rand() % 85 + 5;
    }
    while (kbhit)
    {

        refreshScreen();
        int Xtmp = xj;
        int Ytmp = yj;
        int keyPressed = getch();
        move(keyPressed, &xj, &yj);
        deletejet(Xtmp, Ytmp);
        PrintMyjet();
        if (HP <= 0)
        {
            break;
        }
    }
    system("cls");
    saveData();
    saveDatainFile();
    MatchNumber++;
    gotoxy(width / 2, height / 2);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 192); // reset color to red
    printf("G   A   M   E       O   V   E   R\n");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // reset color to white
    int keyPressed2 = 0;
    while (keyPressed2 != 13)
    {
        system("cls");
        gotoxy(width / 2, height / 2);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5); // reset color to red
        printf("G   A   M   E       O   V   E   R\n");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // reset color to white
        gotoxy(width / 2, height / 2 + 3);
        arrowHere(1, possition2);
        if (possition2 == 1)
        {

            printf("    R   E   S   T   A   R   T\n");
        }
        else
        {
            gotoxy(width / 2, height / 2 + 3);

            printf("RESTART\n");
        }

        gotoxy(width / 2, height / 2 + 4);
        arrowHere(2, possition2);
        if (possition2 == 2)
        {
            printf("    M   E   N   U\n");
        }
        else
        {
            gotoxy(width / 2, height / 2 + 4);
            printf("MENU\n");
        }
        keyPressed2 = getch();
        if ((keyPressed2 == 80) && (possition2 != 2)) // ascii 80 =  Down arrow
            possition2++;
        else if ((keyPressed2 == 72) && (possition2 != 1)) // ascii 72 = up arrow
            possition2--;
        else
            possition2 = possition2;
    }
    if(possition2 == 1)
    {
        
        //restart
        refreshCnt();
        
        option1();
    }
    if(possition2 == 2)
    {
        // option0;
        return;
        possition = 1;
    }
}
