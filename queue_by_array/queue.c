#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

struct queue* 
create_queue(){
    struct queue *q = malloc(sizeof(*q));
    q->cap = QUEUE_DEFAULT_SIZE;
    q->head = q->tail = 0;
    q->data = malloc(sizeof(struct node) * QUEUE_DEFAULT_SIZE);
    return q;
}

void release_queue(struct queue* q){
    free(q->data);
    free(q);
}

void push_queue(struct queue* q, struct node *n){
    q->data[q->tail] = *n;
    if(++q->tail >= q->cap){
        q->tail = 0;
    }
    if(q->tail == q->head){
        struct node *new = malloc(sizeof(struct node) * (q->cap*2));
        int i;
        for(i=0;i<q->cap;i++){
            int index = (q->head+i)%q->cap;
            new[i] = q->data[index];
        }
        q->head = 0;
        q->tail = q->cap;
        q->cap *= 2;
        free(q->data);
        q->data = new;
    }
}

int pop_queue(struct queue* q, struct node* n){
    int ret = 0;
    if(q->head != q->tail){
        *n = q->data[q->head++];
        if(q->head >= q->cap){
            q->head = 0;
        }
        ret = 1;
    }
    return ret;
}

int length_queue(struct queue* q){
    if(q->head <= q->tail){
        return q->tail - q->head;
    } else {
        return q->tail + q->cap - q->head;
    }
}

void dump_queue(struct queue *q){
    int len = length_queue(q);
    printf("队列容量是：%d,%d,%d\n", q->cap, q->head, q->tail);
    printf("队列长度是：%d\n", len);
    int i;
    for(i=0;i<len;i++){
        struct node n = q->data[(q->head+i)%q->cap];
        printf("i=%d id=%d, name=%s\n", i, n.id, n.name);
    }
}
