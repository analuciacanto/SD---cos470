#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <chrono>
#include <ctime>    

#define PORT 8888
#define IP_ADDRESS "127.0.0.1"
#define BUFFER_SIZE 1024

#define REQUEST "1|"
#define GRANT "2|"
#define RELEASE "3|"

using namespace std;

uint64_t getTime() {
  using namespace std::chrono;
  return duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
}

void write()
{
    fstream myfile;
    myfile.open ("resultado.txt",ios_base::app);
    myfile << "Hora: " << getTime() << "  |  " << "Id do processo: " << getpid()  << endl;
    myfile.close();
}

int connect(){

    //Conectar ao servidor (coordenador)

    struct sockaddr_in serv_addr;
    int sock = 0;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
       
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    if(inet_pton(AF_INET, IP_ADDRESS , &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    //conecta com servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    return sock;
}


int request(int sock, int r, int k){

    string message;
    int pid = getpid();
    string mypid = to_string(pid);    
    int n;  

    for (int i = 0; i < r; i++)
    {
        message = REQUEST + mypid;
        cout << "REQUEST" << endl;

        //envia request ao servidor
        n = send(sock , message.c_str(), BUFFER_SIZE, 0);

        if (n < 0)
        {
            printf("Erro escrevendo no socket");
            exit(1);
        } 
        char buffer[BUFFER_SIZE];

        //recebe resposta do servidor
        n = read( sock , buffer, BUFFER_SIZE);
        if (n < 0)
        {
            printf("Erro lendo do socket");
            exit(1);
        }
        string received(buffer);

        //se recebeu GRANT, escreve no arquivo
        if (received.find("2")!= string::npos)
        {
            cout << "GRANT" << endl;
            write();
        }
        sleep(k);

        //envia RELEASE ao servidor
        string release = "3|";
        message = release + mypid;
        cout << "RELEASE" << endl;
        n = send(sock , message.c_str(), BUFFER_SIZE, 0);
        
    }
    return 0;
}
   
int main(int argc, char const *argv[])
{   
    //Pegando variáveis
    // n será iterado pelo script

    int r = atoi(argv[1]);
    int k = atoi(argv[2]);
    int sock = 0;

    sock = connect();  
    request(sock, r, k);
    close(sock);

    return 0;
}