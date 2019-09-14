#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <string.h>

int main(){
    char * a = "~\u256d\u2500\u256e^";
    printf ("%d\n", strlen(a));
}
