#include "gobang_main.h"
#include <stdlib.h>
#include <time.h>
// 各位置上出现黑棋时对应的散列码
unsigned int zobrist_black[BW*BW][2];
// 各位置上出现白棋时对应的散列码
unsigned int zobrist_white[BW*BW][2];
// 反映下棋方轮换的附加散列码
unsigned int zobrist_swap_player[2];

// 对zobrist数组初始化
void init_zobrist() {
    srand(time(NULL));
	zobrist_swap_player[0] = rand();
	zobrist_swap_player[1] = rand();
	for (int pos=0; pos<BW*BW; ++pos) {
		// 因为每步棋都发生下棋方轮换，所以把相应的附加散列码预先叠加在所下棋子上
		zobrist_black[pos][0] = rand() ^ zobrist_swap_player[0];
		zobrist_black[pos][1] = rand() ^ zobrist_swap_player[1];

		zobrist_white[pos][0] = rand() ^ zobrist_swap_player[0];
		zobrist_white[pos][1] = rand() ^ zobrist_swap_player[1];
	}
}


// 更新散列码
void update_zobrist_hashcode(int player,int pos, unsigned int hashcode[]){
	// 如果是黑棋下，则叠加下棋位置上黑棋的散列码
	if (player == BCHESS) {
		hashcode[0] ^= zobrist_black[pos][0];
		hashcode[1] ^= zobrist_black[pos][1];
	// 否则是白棋下，则叠加下棋位置上白棋的散列码
	} else {
		hashcode[0] ^= zobrist_white[pos][0];
		hashcode[1] ^= zobrist_white[pos][1];
	}
}