#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#define OUTPUT printf("%d\n", i)

int main(){
    int i=0; OUTPUT;

    if(fork()){
        wait(NULL);
        i += 2; OUTPUT;
    } else {
        i += 1; OUTPUT; return(0);
    }
}