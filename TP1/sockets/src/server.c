#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
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

int main()
{
    char *ip = "127.0.0.1";
    int port = 5566;

    int server_sock, client_sock;

    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    char buffer[1024];
    int n;

    // Criando o socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    // Configurações do servidor
    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    // Associando o socket criado a porta local do sistema operacional
    n = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Verificando se houve algum erro
    if (n < 0)
    {
        perror("[-]Bind error");
        exit(1);
    }
    printf("[+]Bind to the port number: %d\n", port);

    // Escutar o cliente
    listen(server_sock, 5);
    printf("Listening...\n");

    // loop infinito
    while (1)
    {
        addr_size = sizeof(client_addr);
        // Aceitando o cliente
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_size);
        printf("[+]Client connected.\n");

        bool keepReceiving = true;
        while (keepReceiving)
        {
            // Esvazia o buffer
            bzero(buffer, 1024);

            // Recebe mensagem do cliente
            recv(client_sock, buffer, sizeof(buffer), 0);
            printf("Client: %s\n", buffer);
            int num = atoi(buffer);

            if (num == 0)
            {
                keepReceiving = false;
            }

            if (isPrime(num))
            {
                bzero(buffer, 1024);
                strcpy(buffer, "O número é primo \n");
                printf("Server: %s\n", buffer);
            }
            else
            {
                bzero(buffer, 1024);
                strcpy(buffer, "O número não é primo \n");
                printf("Server: %s\n", buffer);
            }
            send(client_sock, buffer, strlen(buffer), 0);
        }

        close(client_sock);
        printf("[+]Client disconnected.\n\n");
    }
    return 0;
}