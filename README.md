# Trabalhos da disciplina de Sistemas Distribuídos

Projeto para o desenvolvimento dos trabalhos práticos da disciplina de Sistemas Distribuídos ministrada na UFRJ. 

## Compilação e execução

### Clonando o repositório 


```sh
git clone https://github.com/analuciacanto/cos470.git
cd cos470
```

#### Sinais 


```sh
cd TP1/signals/src
gcc consumer.c -o consumer
./pipes

```

Abra outro terminal para executar o produtor


```sh
cd TP1/signals/src
gcc producer.c -o producer
./client

```


#### Pipes

```sh
cd TP1/pipes/src
gcc index.c -o pipes
./pipes

```

#### Sockets

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

