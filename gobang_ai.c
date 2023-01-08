#include "gobang_main.h"
#include "gobang_hash_table.h"

pos global_best_move;
unsigned int hash_code[2];
int history[BW * BW];
int global_score[BW * BW];
int step = 0;
int global_depth;

cscore cs[16] = {
    {.type = "22222",
     .score = 50000,
     .len = 5},
    {.type = "22221",
     .score = 720,
     .len = 5},
    {.type = "12222",
     .score = 720,
     .len = 5},
    {.type = "22122",
     .score = 720,
     .len = 5},
    {.type = "21222",
     .score = 720,
     .len = 5},
    {.type = "22212",
     .score = 720,
     .len = 5},
    {.type = "122221",
     .score = 4320,
     .len = 6},
    {.type = "122211",
     .score = 720,
     .len = 6},
    {.type = "112221",
     .score = 720,
     .len = 6},
    {.type = "122121",
     .score = 720,
     .len = 6},
    {.type = "121221",
     .score = 720,
     .len = 6},
    {.type = "112211",
     .score = 120,
     .len = 6},
    {.type = "112121",
     .score = 120,
     .len = 6},
    {.type = "121211",
     .score = 120,
     .len = 6},
    {.type = "111211",
     .score = 20,
     .len = 6},
    {.type = "112111",
     .score = 20,
     .len = 6}};

const int WEIGHT[BW][BW] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
    {1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1},
    {1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1},
    {1, 2, 3, 4, 5, 5, 5, 5, 5, 5, 5, 4, 3, 2, 1},
    {1, 2, 3, 4, 5, 6, 6, 6, 6, 6, 5, 4, 3, 2, 1},
    {1, 2, 3, 4, 5, 6, 7, 7, 7, 6, 5, 4, 3, 2, 1},
    {1, 2, 3, 4, 5, 6, 7, 8, 7, 6, 5, 4, 3, 2, 1},
    {1, 2, 3, 4, 5, 6, 7, 7, 7, 6, 5, 4, 3, 2, 1},
    {1, 2, 3, 4, 5, 6, 6, 6, 6, 6, 5, 4, 3, 2, 1},
    {1, 2, 3, 4, 5, 5, 5, 5, 5, 5, 5, 4, 3, 2, 1},
    {1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1},
    {1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1},
    {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

int get_score(int cur_player)
{
    int score_black, score_white;
    hash_sum(&score_black, &score_white);
    return cur_player * (score_white - score_black);
}

int get_line_score(hash_tuple t, int player)
{
    // build the str
    int x_pos = t.x_pos == 2 ? -1 : t.x_pos;
    int y_pos = t.y_pos;
    int i, j, k;
    int score = 0;
    char line[BW + 1] = {'\0'};
    char sp[7] = {'\0'};
    for (i = t.x_start, j = t.y_start, k = 0; (x_pos * i) <= (x_pos * t.x_end) && (y_pos * j) <= (y_pos * t.y_end);
         i += x_pos, j += y_pos, k++)
    {
        line[k] = '0' + 1 + BOARD[i][j] * player;
    }
    // printf("%s\n",line);
    score = kmp_find_all(cs, sizeof(cs) / sizeof(cs[0]), line, k);
    return score;
}

// 初始化72个局面
void init_situation()
{
    hash_clear();
    int i, j, hash_value;
    hash_tuple t;
    // add row situation ---
    for (i = 0; i < BW; i++)
    {
        t = get_hash_tuple(i, 0, 0);
        hash_value = get_hash_value(t);
        hash_add(hash_value, 0);
    }
    // add col situation |
    for (i = 0; i < BW; i++)
    {
        t = get_hash_tuple(0, i, 1);
        hash_value = get_hash_value(t);
        hash_add(hash_value, 0);
    }
    /*add LT situation \ */
    for (i = 1; i < BW - 4; i++)
    {
        t = get_hash_tuple(i, 0, 2);
        hash_value = get_hash_value(t);
        hash_add(hash_value, 0);
        t = get_hash_tuple(0, i, 2);
        hash_value = get_hash_value(t);
        hash_add(hash_value, 0);
    }
    t = get_hash_tuple(0, 0, 2);
    hash_value = get_hash_value(t);
    hash_add(hash_value, 0);
    // add LB situation /
    for (i = 4; i < BW; i++)
    {
        t = get_hash_tuple(i, 0, 3);
        hash_value = get_hash_value(t);
        hash_add(hash_value, 0);
        t = get_hash_tuple(BW - 1, i - 4, 3);
        hash_value = get_hash_value(t);
        hash_add(hash_value, 0);
    }
    printf("\ntotal situation:%u\n", hash_count());
}

void init_score()
{
    int i;
    for (i = 0; i < 16; i++)
    {
        get_next(&cs[i]);
    }
}

// update situation
void update_situation(int x, int y)
{
    hash_tuple t;
    int hash_value;
    int score_white, score_black;
    int i, pos = x * BW + y;
    global_score[pos] = 0;
    for (i = 0; i < 4; i++)
    {
        if (i == 2 && abs(x - y) > 10 || i == 3 && abs(x + y - 14) > 10)
            continue;
        t = get_hash_tuple(x, y, i);
        hash_value = get_hash_value(t);
        score_white = get_line_score(t, WCHESS);
        score_black = get_line_score(t, BCHESS);
        global_score[pos] += score_black;
        global_score[pos] += score_white;
        if (hash_replace(hash_value, score_black, score_white) == ERRHVALUE)
        {
            fprintf(stderr, "Error: in update_situation(), update record of (%d,%d) dir=%d fail!\n", x, y, i);
        }
    }
}

int cmp(const void *_a, const void *_b)
{
    pos *a = (pos *)_a;
    pos *b = (pos *)_b;
    int pos_a = a->x * BW + a->y, pos_b = b->x * BW + b->y;
    return history[pos_b] - history[pos_a]; // global_score[pos_b]-global_score[pos_a];//history[pos_b]-history[pos_a];
}

int get_possible_move(pos *move)
{
    int i, j, m, n;
    int size = 0;
    int record[MOVE_ARRAY_SIZE];
    for (i = 0; i < MOVE_ARRAY_SIZE; i++)
    {
        record[i] = 0;
    }
    size = 0;
    for (i = 0; i < BW; i++)
    {
        for (j = 0; j < BW; j++)
        {
            if (BOARD[i][j] == 0)
                continue;
            // 有子位置周围3格范围
            for (m = -1; m < 2; m++)
            {
                for (n = -1; n < 2; n++)
                {
                    if (m == 0 && n == 0)
                        continue;
                    if (i + m < 0 || j + n < 0 || i + m > BW - 1 || j + n > BW - 1)
                        continue;
                    if (BOARD[i + m][j + n] != 0 || record[(i + m) * BW + (j + n)] != 0)
                        continue;
                    record[(i + m) * BW + (j + n)] = 1;
                    move[size].x = i + m;
                    move[size++].y = j + n;
                }
            }
        }
    }

    qsort(move, size, sizeof(pos), cmp);
    return size;
}

int ab_search(int alpha, int beta, int depth, int player)
{
    int flag = FLAG_ALPHA;
    // 散列表查询
    // int a,b;
    int cache_score, cache_depth;
    pos m;
    if ((cache_score = hash_search_situation(hash_code, depth, alpha, beta, &m)) != ERRHVALUE)
    {
        // printf("hit\n");
        if (depth == global_depth)
            global_best_move = m;
        return cache_score;
    }

    if (depth == 0)
    {
        int val = get_score(player);
        hash_update_situation(hash_code, FLAG_EXACT, val, depth, m);
        return val;
    }

    pos move[MOVE_ARRAY_SIZE];
    pos best_move;
    int size = get_possible_move(move);
    int i, original_alpha = alpha;
    int score, best_score = -BEST_SCORE;

    // TODO: 根据历史表和启发式搜索对棋步进行排序
    for (i = 0; i < size; i++)
    {
        MAKE_MOVE(move[i], BOARD, player, hash_code);

        if (win(move[i].x, move[i].y))
        {
            score = get_score(player); // BEST_SCORE-DEPTH+depth;
        }
        else if (best_score == -BEST_SCORE || ((score = -ab_search(-alpha - 1, -alpha, depth - 1, -player)) > alpha && (score) < beta))
        {
            score = -ab_search(-beta, -alpha, depth - 1, -player);
        }
        UNMAKE_MOVE(move[i], BOARD, player, hash_code);

        if (score >= beta)
        {
            if (depth == global_depth)
            {
                global_best_move = move[i];
            }
            hash_update_situation(hash_code, FLAG_BETA, score, depth, move[i]);
            history[move[i].x * BW + move[i].y] += depth*depth;
            // return score;
            return beta;
            // break;
        }
        if (depth == global_depth)
            printf("depth:%d, player:%d, (%d,%c), score: %d, alpha:%d, beta:%d\n", depth, player, BW - move[i].x, 'a' + move[i].y, score, alpha, beta);
        if (score > best_score)
        {
            best_score = score;
            if (score > alpha)
            {
                alpha = score;
                flag = FLAG_EXACT;
                best_move = move[i];
            }
        }
    }
    // 杀棋则根据步数给出评价
    if (best_score == -BEST_SCORE)
        return DEPTH - depth - BEST_SCORE;
    if (alpha != original_alpha)
    {
        // TODO: 将最佳棋步保存到历史表
        history[best_move.x * BW + best_move.y] += depth*depth;
        if (depth == global_depth)
        {
            printf("update!\n");
            history[best_move.x * BW + best_move.y] += depth*depth*depth;
            global_best_move = best_move;
        }
    }
    hash_update_situation(hash_code, flag, best_score, depth, best_move);
    return best_score;
}

// AI function, return chess's position
int AI(int player, int x, int y, int step)
{
    if (step == 1)
    {
        if (x != 7 || y != 7)
        {
            global_best_move.x = 7;
            global_best_move.y = 7;
        }
        else
        {
            global_best_move.x = 6;
            global_best_move.y = 6;
        }
        return get_score(player);
    }
    pos position;
    int score, i;
    for(i=0;i<BW*BW;i++){
        history[i]=0;
    }
    for (global_depth = 1; global_depth < DEPTH + 1; global_depth += 2)
    {
        score = ab_search(-BEST_SCORE, BEST_SCORE, global_depth, player);
        printf("depth:%d, (%d,%c), score:%d\n", global_depth, BW - global_best_move.x, global_best_move.y + 'a', score);
    }
    return score;
}

// AI first
void PVA(int start)
{
    int player = BCHESS;
    int x, real_x = -1, real_y = -1;
    char y;
    int i;
    pos ai_position;
    printf("init hash table...    ");
    init_situation();
    printf("done.\ninit score hash table...\n");
    init_score();
    printf("done.\ninit zobrist...\n");
    printf("done.\n");

    for (i = 0; i < BW * BW; i++)
    {
        history[i] = 0;
    }
    step = 0;
    init();
    hash_code[0] = hash_code[1] = 0;
    if (start)
    {
        real_x = X(8);
        real_y = Y('h');
        set(real_x, real_y, player);
        update_situation(real_x, real_y);
        update_zobrist_hashcode(player, real_x * BW + real_y, hash_code);
        player *= -1;
        step += 1;
    }
    draw(real_x, real_y);
    for (;;)
    {
    input:
        scanf("%d%c", &x, &y);
        real_x = X(x);
        real_y = Y(y);
        if (real_x < 0 || real_y < 0 || real_x > BW - 1 || real_y > BW - 1)
        {
            fprintf(stderr, "Error: wrong position:(%d,%c)\n", x, y);
            goto input;
        }
        if (BOARD[real_x][real_y] != 0)
        {
            fprintf(stderr, "Error: chess piece has already been placed here:(%d,%c)\n", x, y);
            goto input;
        }
        DOWN(real_x, real_y, player, 0);
        // AI part
        int score = AI(player, real_x, real_y, step);
        real_x = global_best_move.x;
        real_y = global_best_move.y;
        DOWN(real_x, real_y, player, 1);
    }
}