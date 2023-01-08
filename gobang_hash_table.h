#ifndef __GOBANG_HASH_TABLE__
#define __GOBANG_HASH_TABLE__

#include "src/uthash.h"
#include "gobang_main.h"

#define ERRHVALUE -100000000
#define FLAG_ALPHA 1
#define FLAG_BETA 2
#define FLAG_EXACT 3

typedef struct{
    int hash_value;//the key
    int score_white;
    int score_black;
    UT_hash_handle hh;
}hash_node;

typedef struct{
    int hash_code_1;//the key
    int hash_code_0;
    int flag;
    int score;
    int depth;
    pos best_move;
    UT_hash_handle hh;
}situ_node;

extern hash_node *hash_table;

hash_tuple get_hash_tuple(int x, int y, int dir);
int get_hash_value(hash_tuple t);
void hash_add(int hash_value, int score);
int hash_search(int hash_value,int player);
int hash_replace(int hash_value,int new_score_black,int new_score_white);
void hash_clear(void);
unsigned int hash_count(void);
void hash_sum(int *score_black,int *score_white);
void hash_printall();
//int hash_search_situation(int hash_code[],int *alpha,int *beta,pos *best_move);
//int hash_update_situation(int hash_code[],int alpha,int beta,pos best_move);

//int hash_update_situation(int hashcode[],int score, int depth, pos best_move);
//int hash_search_situation(int hashcode[],int *score,pos *best_move);

int hash_search_situation(int hashcode[],int depth,int alpha,int beta,pos *best_move);
int hash_update_situation(int hashcode[], int flag, int score, int depth, pos best_move);

#endif