#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void signalHandler_1(int signum)
{
    printf("\n O Sinal 1 foi recebido.\n");
}

void signalHandler_2(int signum)
{
    printf("\n O Sinal 2 foi recebido.\n");
}

void signalHandler_3(int signum)
{
    printf("\n O Sinal 3 foi recebido.\n");
}

int main(int argc, char *argv[])
{

    signal(1, signalHandler_1);
    signal(2, signalHandler_2);
    signal(3, signalHandler_3);

    int waitType;

    printf("\n Digite 0 para executar com busy wait e 1 para executar com blocking wait\n");
    scanf("%d", &waitType);
    printf("\n O id do processo é: %d\n", getpid());

    if (waitType == 0)
    {
        while (1)
        {
            printf("Esperando um sinal em busy wait...\n");
            sleep(2);
        }
    }
    else if (waitType == 1)
    {
        while (1)
        {
            printf("Esperando um sinal em block wait...\n");
            pause();
        }
    }
    else
    {
        printf("Erro na escolha do método de espera");
    }
    return 0;
}
