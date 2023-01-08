#ifndef __GOBANG_MAIN__
#define __GOBANG_MAIN__
#include <stdio.h>

//############### parameter ###############
#define BOARD_WIDTH 15                      // real width
#define BOARD_DRAW_WIDTH BOARD_WIDTH+1      // width for draw
#define BW BOARD_WIDTH
#define BDW BOARD_DRAW_WIDTH
#define WCHESS 1                            // white chess
#define BCHESS -1                           // black chess
#define EMPTY 0                             // no chess
#define DEPTH 7
#define MOVE_ARRAY_SIZE BW*BW
#define BEST_SCORE 50000

//############### macro function ###############
#define max(a,b) (a)>(b)?(a):(b)
//#define abs(a) (a>0)?(a):(-1*(a))
//translate the coordinate from user input to real array index
#define X(x) (BW-x)
#define Y(y) ((y)-'a')
//calculate start and end position
#define START_X_LT(x,y) ((x)<(y))?0:((x)-(y))
#define START_Y_LT(x,y) ((x)<(y))?((y)-(x)):0
#define END_X_LT(x,y)   ((x)<(y))?(14+(x)-(y)):(BW-1)
#define END_Y_LT(x,y)   ((x)>(y))?(14-(x)+(y)):(BW-1)

#define START_X_LB(x,y) ((x)+(y)>14)?(BW-1):((x)+(y))
#define START_Y_LB(x,y) ((x)+(y)>14)?((x)+(y)-14):0
#define END_X_LB(x,y)   START_Y_LB(x,y)
#define END_Y_LB(x,y)   START_X_LB(x,y)
//colorful output
#define CLOSE printf("\033[0m")		 //关闭彩色字体
#define RED printf("\033[1;31m")	 //红色字体
#define GREEN printf("\033[1;32m")	 //绿色字体

#define MAKE_MOVE(pos,board,player,hashcode) do{\
                board[pos.x][pos.y] = player;\
                update_situation(pos.x,pos.y);\
                update_zobrist_hashcode(player,pos.x*BW+pos.y,hashcode);\
            }while(0);
#define UNMAKE_MOVE(pos,board,player,hashcode) do{\
                board[pos.x][pos.y] = 0;\
                update_situation(pos.x,pos.y);\
                update_zobrist_hashcode(player,pos.x*BW+pos.y,hashcode);\
            }while(0);

#define DOWN(x,y,player,is_ai) do{\
            set(x, y, player);\
            update_situation(x, y);\
            update_zobrist_hashcode(player,x*BW+y,hash_code);\
            draw(x, y);\
            step++;\
            printf("%d:(%d,%c)\n",step, BW - x, 'a' + y);\
            if (win(x, y)){\
                printf("%s win!\n", is_ai?"AI":"Player");\
                return;}\
            else\
                player *= -1;\
            }while(0)

//############### structure ###############

// chessman's position
typedef struct position{
    int x;
    int y;
}pos;
// 用于计算哈希值的6元组
typedef struct tuple6{
    char x_start;
    char y_start;
    char x_end;
    char y_end;
    char x_pos;
    char y_pos;
}hash_tuple;
// chess module
typedef struct chess_score{
    char type[7];
    int next[7];
    int len;
    int score;
}cscore;


//############### global arrays ###############
int BOARD[BOARD_WIDTH][BOARD_WIDTH];
char* BOARD_DRAW[BOARD_DRAW_WIDTH][BOARD_DRAW_WIDTH];
extern unsigned int hash_code[];

//############### function declaration ###############
// in gobang_base_func.c
void print_welcome(void);
void init();
void draw(int x,int y);
void set(int x,int y,int color);
int win(int x,int y);
// in gobang_pvp.c
void PVP();
//in gobang_ai.c
void PVA(int start);
//in gobang_kmp.c
void get_next(cscore *mode_string);
int kmp_find_all(cscore S[],int S_size,char T[],int len);
//in gobang_zobrist.c
void init_zobrist();
void update_zobrist_hashcode(int player,int pos, unsigned int hashcode[]);

#endif