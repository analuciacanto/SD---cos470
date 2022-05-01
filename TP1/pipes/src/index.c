#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
// Tamanho do Buffer
#define MAXBUFFER 20

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
    pid_t pid;
    int descritoresPipe[2];
    // descritoresPipe[0] - leitura do pipe
    // descritoresPipe[1] - escrita no pipe
    char buffer[MAXBUFFER];

    // Criando o pipe
    if (pipe(descritoresPipe))
    {
        printf("Pipe falhou.\n");
        return 1;
    }

    // Executando um fork e logo apos verificando se o fork funcionou */
    pid = fork();
    if (pid < 0)
    {
        printf("Fork falhou.\n");
        return 1;
    }

    // Trecho executado pelo pai
    else if (pid > 0)
    {
        // Proceso pai fecha ponta de leitura do pipe
        close(descritoresPipe[0]);
        // Escreve os números e quando acaba o max envia 0
        int max = 0;
        while (max < 50)
        {
            int num = (1 + rand() % 100);
            char snum[20];
            sprintf(snum, "%d", num);
            write(descritoresPipe[1],
                  snum, 20);
            max++;
        }
        write(descritoresPipe[1],
              "0", 20);
        close(descritoresPipe[1]);
    }

    // Trecho executado pelo filho
    else
    {
        // Proceso filho fecha ponta de escrita do pipe
        close(descritoresPipe[1]);
        bool keepReceiving = true;

        // Enquanto não recebe 0 o keepReceiving fica true recebendo do pipe
        while (keepReceiving)
        {
            // Leitura do pipe
            read(descritoresPipe[0], buffer, MAXBUFFER);
            int num = atoi(buffer);
            if (num == 0)
            {
                keepReceiving = false;
            }
            else
            {
                // Para cada número recebido, verifica se é primo.
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
