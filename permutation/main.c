#include <stdio.h>
#include <string.h>

void swap(char* set, int i, int j){
    char tmp = set[i];
    set[i] = set[j];
    set[j] = tmp;
}

void permutation(char* set, int s, int e){
    if(s==e){
        printf("%s\n", set);
        return;
    }
    int i;
    for(i=s;i<e;i++){
        swap(set, i, s);
        permutation(set, s+1, e);
        swap(set, s, i);
    }
}

int main(int argc, char* argv[]){
    char set[] = "abc";
    permutation(set, 0, strlen(set));
    return 0;
}
