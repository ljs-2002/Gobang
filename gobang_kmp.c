#include "gobang_main.h"
#include <omp.h>

void get_next(cscore *mode_string){
    int i,j,k,value;
    int len=mode_string->len;
    mode_string->next[0]=0;
    for(j=2;j<len+1;j++){
        value=1;
        for(k=2;k<j;k++){
           for(i=1;i<k;i++){
               if(mode_string->type[i-1]!=mode_string->type[j+i-k-1]){
                   break;
               }
           }
           if(i==k){
               value=k;
           }
        }
        mode_string->next[j-1]=value;
    }
}

int kmp_search(cscore mode_str,char str[],int len,int *index){
    int i=0,j=0,k=0;
    
    while(i<len){
        if(str[i]==mode_str.type[j]){
            i++;
            j++;
        }else{
            if(j==0){
                i++;
            }else{
                j=mode_str.next[j]-1;
            }
        }
        if(j==mode_str.len){
            index[k++]=i-mode_str.len;
            j=0;
        }
    }
    return k;
}

int kmp_find_all(cscore S[],int S_size,char T[],int len){
    int score=0;
    int i,size;
    int index[7];
    //多线程
    //omp_set_num_threads(4);
    //#pragma omp parallel for private(i,size),reduction(+:score)
    for(i=0;i<S_size;i++){
        size=kmp_search(S[i],T,len,index);
        score += size*S[i].score;
        //printf("thread %d,score = %d\n",omp_get_thread_num(),score);
    }
    return score;
}