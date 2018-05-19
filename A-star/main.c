/*
  A星寻路算法，只能往上下左右4个方向移动，用曼哈顿距离最为启发函数。需要两张表，open列表，存放下一步可经过的点；close列表，存放已经走过的点。
  思路：每次从open列表中选取值最小的为下一步要走的点，直到终点。所以对于open列表有两种选择：一是有序的，这样取最小值容易，但插入新结点后需排序；二是无序的，插入新结点容易，但取最小值需遍历整个表；本实例中采用冒泡思想保证open列表有序。
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define MAZE_WIDTH 7
#define MAZE_HEIGHT 6

// 1起点；2终点；3障碍物；0可经过
#define STARTNODE 1
#define ENDNODE 2
#define BARRIER 3

typedef struct Astar_node{
    int x; //x坐标
    int y; //y坐标
    int type; //类型
    int g; //到起点的值
    int h; //到终点的启发值
    struct Astar_node* parent; //父结点
    int in_open; //在开启列表中
    int in_close; //在关闭列表中
}Astar_node, *pAstar_node;

Astar_node maze[MAZE_WIDTH][MAZE_HEIGHT];
Astar_node *startnode, *endnode, *curnode; //起点，终点，当前点

pAstar_node opened[MAZE_WIDTH*MAZE_HEIGHT]; //开启列表,有序的
pAstar_node closed[MAZE_WIDTH*MAZE_HEIGHT]; //关闭列表
pAstar_node path[MAZE_WIDTH*MAZE_HEIGHT];

int open_node_count; //open列表长度
int close_node_count; //close列表长度

// 启发值：曼哈顿距离
int distance(Astar_node *node1, Astar_node *node2){
    return abs(node2->x - node1->x) + abs(node2->y - node1->y);
}

void dump_opened(){
    int i;
    for(i=0;i<open_node_count;i++){
        printf("%d-%d-%d\n", opened[i]->x, opened[i]->y, opened[i]->g+opened[i]->h);
    }
    printf("\n");
}

// 向open列表插入新结点，插入后要保证有序
void opened_insert_node(Astar_node *node, int w){
    assert(!node->in_open && !node->in_close);
    int i,j;
    for(i=0;i<open_node_count;i++){
        if(w > opened[i]->g+opened[i]->h)
            break;
    }
    for(j=open_node_count-1;j>=i;j--){
        opened[j+1] = opened[j];
    }
    opened[i] = node;
    node->in_open = 1;
    ++open_node_count;
}

// 移除结点，当某一结点有更小的路经时调用
void opened_remove_node(Astar_node *node){
    assert(node->in_open && !node->in_close);
    int i,j;
    for(i=0;i<open_node_count;i++){
        if(opened[i]->x==node->x && opened[i]->y==node->y){
            for(j=i+1;j<open_node_count;j++){
                opened[j-1] = opened[j];
            }
            --open_node_count;
            break;
        }
    }
    node->in_open = 0;
}

// 获取上下左右4个相邻结点
void get_neighbours(Astar_node *curr_node){
    int x=curr_node->x, y=curr_node->y;
    int offset[4][2] = {
        {0,1},{0,-1},{1,0},{-1,0}
    };
    int i;
    for(i=0;i<4;i++){
        int nx=x+offset[i][0];
        int ny=y+offset[i][1];
        if(nx>=0 && nx<MAZE_WIDTH && ny>=0 && ny<MAZE_HEIGHT){
            Astar_node *node = &maze[nx][ny];
            if(node->type != BARRIER && !node->in_close){ //不是障碍物且不在close列表里
                int g = curr_node->g + 1;
                int h = distance(node, endnode);
                int need_insert = !node->in_open;
                if(node->in_open && g+h < node->g+node->h){ //已经在open列表，但有更小的路径值
                    opened_remove_node(node);
                    need_insert = 1;
                }
                if(need_insert){ //插入到open列表
                    opened_insert_node(node, g+h);
                    node->parent = curr_node;
                    node->g = g;
                    node->h = h;
                }
            }
        }
    }
}

int main(int args, char* argv[]){
    /* 左下角是(0,0)点
      0 0 0 0 0 0 0
      0 0 0 3 0 0 0
      0 0 0 3 0 0 0
      0 1 0 3 0 0 0
      0 3 0 3 0 2 0
      0 0 0 0 0 0 0
    */
    int data[MAZE_WIDTH][MAZE_HEIGHT] = {
        {0,0,0,0,0,0},
        {0,3,1,0,0,0},
        {0,0,0,0,0,0},
        {0,3,3,3,3,0},
        {0,0,0,0,0,0},
        {0,2,0,0,0,0},
        {0,0,0,0,0,0},
    };
    int i,j;
    // 构造数据
    for(i=0;i<MAZE_WIDTH;i++){
        for(j=0;j<MAZE_HEIGHT;j++){
            maze[i][j].x = i;
            maze[i][j].y = j;
            maze[i][j].type = data[i][j];
            maze[i][j].g = 0;
            maze[i][j].h = 0;
            maze[i][j].parent = NULL;
            maze[i][j].in_open = 0;
            maze[i][j].in_close = 0;
            if(maze[i][j].type == STARTNODE){
                maze[i][j].in_open = 1;
                startnode = &maze[i][j];
            }else if(maze[i][j].type == ENDNODE){
                endnode = &maze[i][j];
            }
        }
    }
    startnode->h = distance(startnode, endnode);
    open_node_count = 0;
    close_node_count = 0;

    // 开始寻路
    int in_found = 0;
    opened[open_node_count++] = startnode; //起点加入到opend
    while(open_node_count>0){
        curnode = opened[--open_node_count]; //从opend取g+h最小的直
        if(curnode->type == BARRIER){ //障碍物
            continue;
        }
        curnode->in_open = 0; //当前点加入到close列表
        closed[close_node_count++] = curnode;
        curnode->in_close = 1;
        if(curnode->type == ENDNODE){ //到达终点
            in_found = 1;
            break;
        }
        get_neighbours(curnode);
    }
    int top=0;
    if(in_found){ //从终点反推到起点
        Astar_node *node = curnode;
        while(node != startnode){
            path[top++] = node;
            node = node->parent;
        }
        path[top] = startnode;
        for(i=top;i>=0;i--){
            printf("(%d,%d)", path[i]->x, path[i]->y);
            if(i>0){
                printf("--->");
            }
        }
        printf("\n");
    } else {
        printf("no path!\n");
    }
    return 0;
}
