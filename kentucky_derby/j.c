#include <stdio.h>
#include <stdlib.h>

void add_one(int *i){
    ++(*(i + 2));
    ++(*(i + 2));
}

int main(){
    int *i = (int *) calloc (4, sizeof(int));

    add_one(i);

    printf ("%d\n", *(i +2));
    exit (0);
}
