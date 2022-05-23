#include <numeric>
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>
#include <iostream>
#include <atomic>
#include "./utils.cpp"
#include <cstring>

using namespace std;

//Mudar N de acordo com os casos de teste.
//Testar com os casos N = 10^7, N = 10^8 e N = 10^9.

#define N 10
// K = número de threads que vão trabalhar simultaneamente
#define K 2


int totalSum = 0;    //Armazena  a soma total.
atomic_flag lock_stream = ATOMIC_FLAG_INIT;  



void acquire(atomic_flag *lock)
{
    while (lock->test_and_set());
}

void release(atomic_flag *lock)
{
    lock->clear();
}

vector<int> populateVector(int size)
{   //Gerando vetores de tamanho size com números aleatróios entre [-100,100]
    vector<int> array;
    for (int i = 0; i < size; i++)
    {
        int num = (rand() % 201) - 100;  
        array.push_back(num);
    }
    return array;
}

void sumVector(vector<int> vectorToSum, int numThreads ){
    int sum = accumulate(vectorToSum.begin(), vectorToSum.end(), 0);  // Somando o vetor
    acquire(&lock_stream);      //Bloquear a área crítica
    totalSum += sum;            //Acessa o endereço de memória compartilhado.
    release(&lock_stream);      //Libera a área crítica
}


int main(){

    int numberOfExecutions = 2;  //Quantas
    double totalTime = 0;


    int numberOfThreads = K;
    vector<int> numbers = populateVector(N);

     for (int i = 1; i <= numberOfExecutions; i++)    {

        int vectorSize = N / K;   //Utilizando a sugestão do professor de dividir  N/K

        vector<vector<int>> vectorOfChunks = splitVectorIntoChunks(numbers, vectorSize);
        //retorna vários vetores
        
        vector<thread> v;

        // Começando a contar o tempo
        auto start = chrono::system_clock::now();

        for (int i = 0; i < K ; i++)
        {
            vector<int> chunk = vectorOfChunks[i];            
            thread thread_n(sumVector, chunk, i);
            v.push_back((thread &&)(thread_n));
        }

        for (int i = 0; i < K; i++)
        {
            // Must wait for each thread to finish
            v[i].join();
        }

        // Ending to measure time
        auto end = chrono::system_clock::now();

        chrono::duration<double> elapsed = end - start;

        totalTime = totalTime + elapsed.count();
        printf("Número de threads %i\n" , K);
        printf("Número de elementos no vetor inicial %i\n" , N);
        printf("Número de elementos no vetor dividido %i\n" , vectorSize);
        printf("Soma acumulada %i\n", totalSum);          
        printf("Total time: %f\n", totalTime );
                        

     }
     
    return 0;
}