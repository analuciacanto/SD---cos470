#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

int isPrime(int n)
{
    int i;
    for (i = 2; i <= n / 2; ++i)
    {
        if (n % i == 0)
            return 0;
    }

    return 1;
}

int main(void)
{
    int maxBuffer = 20;
    pid_t pid;
    int descritoresPipe[2];
    // descritoresPipe[0] - leitura do pipe
    // descritoresPipe[1] - escrita no pipe
    char buffer[maxBuffer];

    if (pipe(descritoresPipe))
    {
        printf("Pipe falhou.\n");
        return 1;
    }

    pid = fork();
    if (pid < 0)
    {
        printf("Fork falhou.\n");
        return 1;
    }

    // Trecho executado pelo pai
    else if (pid > 0)
    {
        close(descritoresPipe[0]);
        int max = 0;

        printf("Quantos números aleatórios gostaria de gerar e conferir se é primo?\n");
        scanf("%d", &max);

        while (max > 0)
        {
            int num = (1 + rand() % 100);
            char snum[20];
            sprintf(snum, "%d", num);
            write(descritoresPipe[1],
                  snum, 20);
            max--;
        }
        write(descritoresPipe[1],
              "0", 20);
        close(descritoresPipe[1]);
    }

    // Trecho executado pelo filho
    else
    {
        close(descritoresPipe[1]);
        bool keepReceiving = true;

        while (keepReceiving)
        {
            read(descritoresPipe[0], buffer, maxBuffer);
            int num = atoi(buffer);
            if (num == 0)
            {
                keepReceiving = false;
            }
            else
            {
                if (isPrime(num))
                {
                    printf("O número %d  é primo \n", num);
                }
                else
                {
                    printf("O número %d não é primo \n", num);
                }
            }
        }
        close(descritoresPipe[0]);
        return 0;
    }
}
