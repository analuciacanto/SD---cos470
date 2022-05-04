# Trabalhos da disciplina de Sistemas Distribuídos

Projeto para o desenvolvimento dos trabalhos práticos da disciplina de Sistemas Distribuídos ministrada na UFRJ. 

## Compilação e execução

### Sinais 
Em construção

### Pipes

```sh
cd TP1/pipes/src
gcc index.c -o pipes
./pipes

```

### Sockets

Pra subir o servidor 

```sh
cd TP1/sockets/src
gcc server.c -o server
./server

```

Abra outro terminal para executar o cliente


```sh
cd TP1/sockets/src
gcc client.c -o client
./client

```

## Configuração para Desenvolvimento

Para instalação do compilador no ambiente linux (ubuntu), podem ser efetudas as seguintes etapas:

```sh
sudo apt update
sudo apt install build-essential
```

## Histórico de lançamentos

* 0.0.1
    * Primeiro trabalho prático

