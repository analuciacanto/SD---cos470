#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{

    char *ip = "127.0.0.1";
    int port = 5566;

    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[1024];
    int n;

    // Criando o socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    // Configurações do servidor

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr.s_addr = inet_addr(ip);

    // Conectando ao servidor criado no server
    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    printf("Connected to the server.\n");

    int max = 0;
    while (max < 10)
    {
        // Gerando os número
        int num = (1 + rand() % 100);
        char snum[20];
        sprintf(snum, "%d", num);
        // Enviando os números
        bzero(buffer, 1024);
        strcpy(buffer, snum);
        send(sock, buffer, strlen(buffer), 0);
        printf("Client: %s\n", buffer);

        bzero(buffer, 1024);
        recv(sock, buffer, sizeof(buffer), 0);
        printf("Server: %s\n", buffer);

        max++;
    }
    bzero(buffer, 1024);
    strcpy(buffer, "0");
    send(sock, buffer, strlen(buffer), 0);
    printf("Client: %s\n", buffer);

    close(sock);
    printf("Disconnected from the server.\n");

    return 0;
}