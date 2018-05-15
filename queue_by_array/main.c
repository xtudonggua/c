#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

void push(struct queue *q, int id, char* name){
    struct node n;
    n.id = id;
    n.name = name;
    push_queue(q, &n);
}

void pop(struct queue *q){
    struct node n;
    int ret = pop_queue(q, &n);
    if(ret){
        //printf("pop = %d, %s\n", n.id, n.name);
    }
}

static char* names[] = {"aaa", "bbb", "ccc", "ddd"};

int main(int argc, char* args[]){
    struct queue *q = create_queue();
    int i;
    for(i=0;i<1<<10;i++){
        if(i%10==0){
            pop(q);
        } else {
            push(q, i, names[i%4]);
        }
    }
    dump_queue(q);
    release_queue(q);
    return 0;
}
