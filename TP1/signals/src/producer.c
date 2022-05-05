#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>

int main(){
    int signal;
    pid_t pid;
    printf("Digite o id do processo:\n");
    scanf("%d",&pid);
    printf("Digite o sinal que deseja enviar:\n");
    scanf("%d",&signal);
    struct stat sts;
    if (stat("/proc/<pid>", &sts) == -1 && errno == ENOENT) {
        perror("pid não existe");
    }
    else{
        kill(pid,signal);
    }
    
    return 0;
}
