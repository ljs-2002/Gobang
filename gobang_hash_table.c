#include "gobang_main.h"
#include "gobang_hash_table.h"

hash_node *hash_table = NULL;
situ_node *situ_table = NULL;
hash_tuple get_hash_tuple(int x, int y, int dir)
{
    /*
     * dir = 0 : row ---
     * dir = 1 : col |
     * dir = 2 : LT \
     * dir = 3 : LB /
     */

    hash_tuple t;
    switch (dir)
    {
    case 0:
        t.x_start = t.x_end = x;
        t.y_start = 0;
        t.y_end = BW - 1;
        t.x_pos = 0;
        t.y_pos = 1;
        break;
    case 1:
        t.x_start = 0;
        t.x_end = BW - 1;
        t.y_start = t.y_end = y;
        t.x_pos = 1;
        t.y_pos = 0;
        break;
    case 2:
        t.x_start = START_X_LT(x, y);
        t.y_start = START_Y_LT(x, y);
        t.x_end = END_X_LT(x, y);
        t.y_end = END_Y_LT(x, y);
        t.x_pos = t.y_pos = 1;
        break;
    case 3:
        t.x_start = START_X_LB(x, y);
        t.y_start = START_Y_LB(x, y);
        t.x_end = END_X_LB(x, y);
        t.y_end = END_Y_LB(x, y);
        t.x_pos = 2;
        t.y_pos = 1;
        break;
    default:
        break;
    }
    return t;
}

int get_hash_value(hash_tuple t)
{
    int hash_value = 0;
    hash_value = t.x_start << 20 | t.y_start << 16 | t.x_end << 12 | t.y_end << 8 | t.x_pos << 4 | t.y_pos;
    return hash_value;
}

void hash_add(int hash_value, int score)
{
    hash_node *s;
    HASH_FIND_INT(hash_table, &hash_value, s);
    if (!s)
    {
        s = (hash_node *)malloc(sizeof(hash_node));
        s->hash_value = hash_value;
        HASH_ADD_INT(hash_table, hash_value, s);
    }
    s->score_black = s->score_white = score;
}

int hash_search(int hash_value, int player)
{
    hash_node *s;
    HASH_FIND_INT(hash_table, &hash_value, s);
    if (!s)
    {
        fprintf(stderr, "Error: in hash_search(), cant find record with hash value:%d\n", hash_value);
        return ERRHVALUE;
    }

    return (player == WCHESS) ? s->score_white : s->score_black;
}

int hash_search_situation(int hashcode[],int depth,int alpha,int beta,pos *best_move){
    situ_node *s;
    int h1 = hashcode[1];
    HASH_FIND_INT(situ_table,&h1,s);
    if(!s||s->hash_code_0!=hashcode[0]||s->depth<depth){
        return ERRHVALUE;
    }
    //*alpha = s->alpha;
    //*beta = s->beta;
    //best_move->x = s->best_move.x;
    //best_move->y = s->best_move.y;
    if(s->flag==FLAG_EXACT)
        return s->score;
    if(s->flag==FLAG_ALPHA&&s->score<=alpha)
        return alpha;
    if(s->flag==FLAG_BETA&&s->score>=beta)
        return beta;
    return ERRHVALUE;
}

int hash_replace(int hash_value, int new_score_black, int new_score_white)
{
    hash_node *s;
    HASH_FIND_INT(hash_table, &hash_value, s);
    if (!s)
    {
        fprintf(stderr, "Error: in hash_replace(), can't find record with hash value:%d\n", hash_value);
        return ERRHVALUE;
    }
    s->score_black = new_score_black;
    s->score_white = new_score_white;
    return 0;
}
//最新替换
int hash_update_situation(int hashcode[], int flag, int score, int depth, pos best_move){
    situ_node *s;
    int h1 = hashcode[1];
    HASH_FIND_INT(situ_table,&h1,s);
    if(!s){
        s = (situ_node *)malloc(sizeof(situ_node));
        s->hash_code_1 = hashcode[1];
        HASH_ADD_INT(situ_table,hash_code_1,s);
    }
    s->hash_code_0 = hashcode[0];
    //s->alpha = alpha;
    //s->beta = beta;
    s->depth = depth;
    s->score = score;
    s->flag = flag;
    s->best_move.x = best_move.x;
    s->best_move.y = best_move.y;
}

void hash_clear()
{
    hash_node *cur, *tmp;
    HASH_ITER(hh, hash_table, cur, tmp)
    {
        HASH_DEL(hash_table, cur);
        free(cur);
    }
    hash_table = NULL;
}

unsigned int hash_count(void)
{
    return HASH_COUNT(hash_table);
}

void hash_sum(int *score_black, int *score_white)
{
    hash_node *s;
    *score_white = 0;
    *score_black = 0;
    for (s = hash_table; s != NULL; s = s->hh.next)
    {
        *score_black += s->score_black;
        *score_white += s->score_white;
    }
}

void hash_printall()
{
    hash_node *s;
    for (s = hash_table; s != NULL; s = s->hh.next)
    {
        printf("hash value: %d,\tscore_black=%d,\tscore_white=%d\n", s->hash_value,s->score_black,s->score_white);
    }
}