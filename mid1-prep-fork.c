#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    int i = 1;
    pid_t p1 = fork();

    if (p1<0){
        fprintf(stderr, "fork failed");
        exit(-1); 
    } else if(p1){
        printf("main parent\n");
        i++;}
    else{
        pid_t p2 = fork();
        if(p2<0){
            fprintf(stderr, "fork failed");
            exit(-1); }
        else if(p2){
            printf("child\n");
            i--;}
        else 
            printf("child child\n");
            i++;
    }
    printf("%d\n", i);
    return(0);
}
