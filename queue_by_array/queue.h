#define QUEUE_DEFAULT_SIZE 32

struct node {
    int id;
    char *name;
};

struct queue{
    int cap;
    int head;
    int tail;
    struct node *data;
};

struct queue* create_queue();
void release_queue(struct queue*);
void push_queue(struct queue*, struct node*);
int pop_queue(struct queue*, struct node*);
int length_queue(struct queue*);
void dump_queue(struct queue*);
