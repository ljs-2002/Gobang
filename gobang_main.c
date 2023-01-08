#include <stdio.h>

#include "gobang_main.h"

int main(){
    int game_mode;
    init_zobrist();
    print_welcome();
    
    do{
cycle:
        printf("0. exit\n");
        printf("1. person vs person\n");
        printf("2. person vs AI\n");
        printf("3. AI vs person\n");
        printf("please input game mode: ");
        scanf("%d",&game_mode);
        //if input wrong game mode, output an error and re input
        if(game_mode<0||game_mode>3){
            fprintf(stderr,"Error: wrong game mode!\nplease input a correct game mode\n");
            goto cycle;
        }
        
        switch (game_mode)
        {
        case 0:
            goto end;
            break;
        case 1:
            PVP();
            break;
        case 2:
            PVA(0);
            break;
        case 3:
            PVA(1);
            break;
        default:
            break;
        }
    }while(game_mode);

end:
    return 0;
}