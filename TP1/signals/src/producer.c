#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

int main()
{
    pid_t pid;

    int signal;

    printf("Insira o número do processo que gostaria de enviar o sinal\n");
    scanf("%d", &pid);

    printf("Insira o sinal que gostaria de enviar ao processo\n");
    scanf("%d", &signal);

    // Enviando o sinal para o processo escolhido
    kill(pid, signal);

    // Tratando erros.
    switch (errno)
    {
    case EPERM:
        printf("Sem permissão para enviar\n");
        break;
    case EINVAL:
        printf("O sinal é inválido\n");
        break;
    case ESRCH:
        printf("O processo é inválido\n");
        break;
    }
    return 0;
}
