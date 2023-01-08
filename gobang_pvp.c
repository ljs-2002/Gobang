#include "gobang_main.h"
#include "gobang_hash_table.h"

void PVP(void){
    int y,chess;
    char x;
    int real_x,real_y;
    init();
    draw(-1,-1);
    chess = BCHESS;
    unsigned int h[2];
    pos m;
    int a,b;
    h[0]=h[1]=0;
    for(;;){
input:
        scanf("%d%c",&x,&y);
        real_x = X(x);
        real_y = Y(y);
        if(real_x<0||real_y<0||real_x>BW-1||real_y>BW-1){
            fprintf(stderr,"Error: wrong position:(%d,%c)\n",x,y);
            goto input;
        }
        if(BOARD[real_x][real_y]!=0){
            fprintf(stderr,"Error: chess piece has already been placed here:(%d,%c)\n",x,y);
            goto input;
        }
        set(real_x,real_y,chess);
        draw(real_x,real_y);
        m.x = real_x;
        m.y = real_y;
        update_zobrist_hashcode(chess,real_x*BW+real_y,h);
        printf("hash code:%u,%u\n",h[1],h[0]);
        if(win(real_x,real_y)){
            printf("%s win!\n",chess==WCHESS?"white":"black");
            return;
        }else
            chess*=-1;
    }
}