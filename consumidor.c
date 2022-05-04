#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

void my_handler1(int signum){
    printf("Dentro da função 1\n");

}
void my_handler2(int signum){
    printf("Dentro da função 2\n");
}
void my_handler3(int signum){
    printf("Dentro da função 3\n");

}
int main(){
    int waiting;
    signal(SIGINT,my_handler1);
    signal(SIGTERM,my_handler2);
    signal(SIGILL,my_handler3); //registrando sinais
    srand(time(0));
    waiting = rand(1);
    if(waiting == 0){
        while(1){
	    printf("Dentro da main\n"); //busy
	    sleep(1); 
	  }
    }
    else{
        while(1){
        printf("Dentro da main\n");
        pause(); //blocking
        }
    }

    return 0;
}
