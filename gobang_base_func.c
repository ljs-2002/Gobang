#include "gobang_main.h"
#include <stdio.h>
#include <stdlib.h> // for itoa()
#include <string.h>

// ############### symbol ###############
const char *WHITE = "●", *BLACK = "○";
const char *LT = "┌", *TOP = "┬", *RT = "┐";
const char *LEFT = "├", *CENTER = "┼", *RIGHT = "┤";
const char *LB = "└", *BUTTON = "┴", *RB = "┘";

void print_welcome(void)
{
    printf("################ Gobang #################\n");
    printf("############## Author: LJS ##############\n");
}

void init(void)
{
    int i, j;
    char *y_co;
    char *x_co;
    // init the board
    for (i = 0; i < BW; i++)
    {
        for (j = 0; j < BW; j++)
        {
            BOARD[i][j] = EMPTY;
        }
    }

    // init the draw board
    // four corners
    BOARD_DRAW[0][1] = LT;
    BOARD_DRAW[0][BDW - 1] = RT;
    BOARD_DRAW[BDW - 2][1] = LB;
    BOARD_DRAW[BDW - 2][BDW - 1] = RB;

    // top and button
    for (i = 2; i < BDW - 1; i++)
    {
        BOARD_DRAW[0][i] = TOP;
        BOARD_DRAW[BDW - 2][i] = BUTTON;
    }
    // left and right
    for (i = 1; i < BDW - 2; i++)
    {
        BOARD_DRAW[i][1] = LEFT;
        BOARD_DRAW[i][BDW - 1] = RIGHT;
    }
    // center
    for (i = 1; i < BDW - 2; i++)
    {
        for (j = 2; j < BDW - 1; j++)
        {
            BOARD_DRAW[i][j] = CENTER;
        }
    }
    // y coordinate
    for (i = 0; i < BDW - 1; i++)
    {
        y_co = (char *)malloc(sizeof(char) * 3);
        sprintf(y_co, "%2d", BW - i);
        BOARD_DRAW[i][0] = y_co;
    }
    // x coordinate
    for (i = 1; i < BDW; i++)
    {
        x_co = (char *)malloc(sizeof(char) * 2);
        x_co[0] = 'a' + i - 1;
        x_co[1] = '\0';
        BOARD_DRAW[BDW - 1][i] = x_co;
    }

    BOARD_DRAW[BDW - 1][0] = "  ";
}

void draw(int x,int y)
{
    int i, j;
    for (i = 0; i < BDW; i++)
    {
        for (j = 0; j < BDW; j++)
        {
            // chessman
            if (strcmp(BOARD_DRAW[i][j], WHITE) == 0 ||
                strcmp(BOARD_DRAW[i][j], BLACK) == 0)
            {
                // set last put chessman different color
                if(x!=-1&&i==x&&j==y+1)
                    GREEN;
                printf("%s ", BOARD_DRAW[i][j]);
                CLOSE;
            }
            else
            {
                printf("%s ", BOARD_DRAW[i][j]);
            }
            if (j == BDW - 1)
            {
                printf("\n");
            }
        }
    }
}

//x,y: coordinate in BOARD
void set(int x,int y,int color){
    if(x<0||x>BW-1||y<0||y>BW-1){
        fprintf(stderr,"Error: in set(): error coordinate:(%d,%d)\n",x,y);
        return ;
    }
    switch (color)
    {
    case WCHESS:
        BOARD[x][y] = color;
        BOARD_DRAW[x][y+1] = WHITE;
        break;
    
    case BCHESS:
        BOARD[x][y] = color;
        BOARD_DRAW[x][y+1] = BLACK;
        break;
    default:
        fprintf(stderr,"Error in set(): error color:%d\n",color);
        break;
    }

}

// judge whether win
int win(int x,int y){
    int color = BOARD[x][y];
    int i,j;
    int row_start;
    int colum_start;
    int score;
    int ret = 0;
    // check row ---
    for(j = 0;j<5;j++){
        //transborder
        if(y-j<0)
            break;
        if(y-j+4>BW-1)
            continue;
        score=BOARD[x][y-j+4]+BOARD[x][y-j+3]+BOARD[x][y-j+2]+BOARD[x][y-j+1]+BOARD[x][y-j];

        if(score == 5*color){
            ret = 1;
            goto end;
        }
    }
    //check colum |
    for(i = 0;i<5;i++){
        if(x-i<0)
            break;
        if(x-i+4>BW-1)
            continue;
        score = BOARD[x-i+4][y]+BOARD[x-i+3][y]+BOARD[x-i+2][y]+BOARD[x-i+1][y]+BOARD[x-i][y];
        if(score == 5*color){
            ret = 1;
            goto end;
        }
    }
    //check RT to LB /
    for(i = 0;i<5;i++){
        if(x-i<0||y+i>BW-1)
            break;
        if(x-i+4>BW-1||y+i-4<0)
            continue;
        score = BOARD[x-i+4][y+i-4]+BOARD[x-i+3][y+i-3]+BOARD[x-i+2][y+i-2]+BOARD[x-i+1][y+i-1]+BOARD[x-i][y+i];
        if(score == 5*color){
            ret = 1;
            goto end;
        }
    }
    //check LT to RB
    for(i = 0;i<5;i++){
        if(x-i<0||y-i<0)
            break;
        if(x-i+4>BW-1||y-i+4>BW-1)
            continue;
        score = BOARD[x-i+4][y-i+4]+BOARD[x-i+3][y-i+3]+BOARD[x-i+2][y-i+2]+BOARD[x-i+1][y-i+1]+BOARD[x-i][y-i];
        if(score == 5*color){
            ret = 1;
            goto end;
        }
    }

end:
    return ret;
}
