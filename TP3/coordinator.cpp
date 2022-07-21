#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> 
#include <arpa/inet.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> 


#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <queue>
#include <map>
#include <sstream>
#include <fstream>

#define TRUE 1
#define FALSE 0

#define IP_ADDRESS "127.0.0.1"
#define PORT 8888

#define BUFFER_SIZE 1024

using namespace std;

uint64_t getTime() {
  using namespace std::chrono;
  return duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
}

void writeLog(string pid, string message)
{	string msg = "";
	if (message == " 1")
		msg = "REQUEST";
	else if (message == " 2")
		msg = "GRANT";
	else if (message == " 3")
		msg= "RELEASE";
	
    fstream myfile;
    myfile.open ("console.log",ios_base::app);
	myfile << "Hora: " << getTime() << "  |  " << "Mensagem: " << msg << "  |  " << "Processo: " << pid << endl;
    myfile.close();
}


class MessagesManager
{	
	//Mutex para gerenciar a região crítica
	mutex mutexem;

	queue<pair<string, int>> q;
	bool lock = false;
	map<string, int> granted; //

	void grant()
	{
		 //Envia grant ao processo
		lock = true;
		pair<string, int> next = q.front();
		q.pop();
		string message = "2";

		//envia GRANT ao processo
		send(get<1>(next), message.c_str(), BUFFER_SIZE, 0);

		writeLog(get<0>(next), " 2");

		if (granted.find(get<0>(next)) == granted.end())
		{
			granted[get<0>(next)] = 1;
		}
		else
		{
			granted[get<0>(next)] += 1;
		}
	}
	
	public:
		MessagesManager(){};
		void request(string pid, int n_socket)
		{
			mutexem.lock();

			writeLog(pid , " 1");
			q.push(make_pair(pid, n_socket));
			if (granted.find(pid) == granted.end())
			{
				granted[pid] = 0;
			}
			if(!lock)
			{
				grant();
			}
			mutexem.unlock();
		}

		void release(string pid)
		{
			mutexem.lock();
			writeLog(pid , " 3");
			if (q.empty())
			{
				lock = false;
			}
			else{
				grant();
			}
			mutexem.unlock();
		}
};

MessagesManager cmutex;

int server()
{
	int opt = TRUE;
	int coordinator_socket , addrlen , new_socket ,
		max_clients = 129 , activity, i , valread , sd;
	int max_sd;
	int manager_socket;
	struct sockaddr_in address;

	vector<int> socket_queue;
		
	char buffer[BUFFER_SIZE]; 
		
	fd_set read_fd_set;
			
	for (i = 0; i < max_clients; i++)
	{
		socket_queue.push_back(0);
	}
		
	//Criando o socket coordenador
	if( (coordinator_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}	

	if( setsockopt(coordinator_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
		sizeof(opt)) < 0 )
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
		
	if (bind(coordinator_socket, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	
	if (listen(coordinator_socket, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
		
	addrlen = sizeof(address);
	puts("Waiting for connections ...");
	
	while(TRUE)
	{ /* Trechos de código retirado de: https://www.gta.ufrj.br/ensino/eel878/sockets/selectman.html*/

		FD_ZERO(&read_fd_set);
	
		FD_SET(coordinator_socket, &read_fd_set);
		max_sd = coordinator_socket;
			
		for ( i = 0 ; i < max_clients ; i++)
		{
			sd = socket_queue[i];
				
			if(sd > 0)
				FD_SET( sd , &read_fd_set);
				
			if(sd > max_sd)
				max_sd = sd;
		}
	
		//Espera por uma atividade em um dos sockets
		/*Código com ajuda do vídeo https://www.youtube.com/watch?v=Y6pFtgRdUts*/
		activity = select( max_sd + 1 , &read_fd_set , NULL , NULL , NULL);
	
		if ((activity < 0) && (errno!=EINTR))
		{
			printf("select error");
		}
			
		if (FD_ISSET(coordinator_socket, &read_fd_set))
		{
			if ((new_socket = accept(coordinator_socket,
					(struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
			{
				perror("accept");
				exit(EXIT_FAILURE);
			}
			

			//Cada vez que o coordinator_socket recebe uma nova conexão, adiciona na socket_queue
			for (i = 0; i < max_clients; i++)
			{
				if( socket_queue[i] == 0 )
				{
					socket_queue[i] = new_socket;
						
					break;
				}
			}
		}
			
		//Caso não seja uma conexão, é uma mensagem, portanto envia a mensagem para o manager lidar. 
		for (i = 0; i < max_clients; i++)
		{
			sd = socket_queue[i];
				
			if (FD_ISSET( sd , &read_fd_set))
			{
				if ((valread = read( sd , buffer, BUFFER_SIZE)) == 0)
				{
					getpeername(sd , (struct sockaddr*)&address , \
						(socklen_t*)&addrlen);
				
					close( sd );
					socket_queue[i] = 0;
				}
					
				else
				{
					string received(buffer);
					if (received.find("manager")!= string::npos)
					{
						manager_socket = sd;
					}
					else
					{
						string str_socket = to_string(sd);
						string answer = received + "|" + str_socket;						
						send(manager_socket , answer.c_str() , BUFFER_SIZE , 0 );
					}
				}
			}
		}
	}
	return 0;
}


int connect(){
	int server_sock = 0;
    struct sockaddr_in serv_addr;

    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    if(inet_pton(AF_INET, IP_ADDRESS, &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(server_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
	printf("Servidor pronto");
	return server_sock;

}

int manager()
{
	//O manager se conecta como cliente ao socket coordenador

	int server_sock = 0, n;
	server_sock = connect();
	string str_manager = "manager";
	
	//Envia mensagem ao socket se identificando e mostrando que está ali.
	n = send(server_sock , str_manager.c_str(), BUFFER_SIZE, 0);
		if (n < 0)
		{
			printf("Erro escrevendo no socket");
			exit(1);
		} 
	char buffer[BUFFER_SIZE];
	int count = 0;
	while(TRUE)
	{
		//Pega a mensagem do servidor
		n = read( server_sock , buffer, BUFFER_SIZE);
		if (n < 0)
		{
			printf("Erro lendo do socket");
			exit(1);
		}
		string server_response(buffer);

		stringstream test(server_response);
		string segment;

		vector<string> messageSplit;

		while(getline(test, segment, '|'))
		{
			//Separando a mensagem pela | e adicionando em uma lista
			messageSplit.push_back(segment);
		}
		if (messageSplit[0] == "1") 
		{
			cmutex.request(messageSplit[1],stoi(messageSplit[2]));
		}
		else if (messageSplit[0] == "3") 
		{
			cmutex.release(messageSplit[1]);
		}
	}
	close(server_sock);
	return 0;
}


int main(int argc , char *argv[])
{
	thread server_t (server);
	thread manager_t (manager);
    server_t.join();
	manager_t.join();
	return 0;
}
