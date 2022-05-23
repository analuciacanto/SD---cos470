#include <numeric>
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>
#include <iostream>
#include <atomic>
#include "./utils.cpp"
#include <cstring>
#include "gnuplot-iostream.h"
#include "./supportLib.cpp"
#include "./pbPlots.cpp"

using namespace std;

int totalSum = 0;    //Acumulador compartilhado
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
    acquire(&lock_stream);  
    //Região crítica  
    totalSum += sum;          
    release(&lock_stream);     
}


float adderWithSpinlocks(vector<int>  numbersVector, int K, int N) {

        double totalTime = 0;


        int vectorSize = N / K;   //Utilizando a sugestão do professor de dividir  N/K


        //Dividindo o vetor em subvetores
        vector<vector<int>> vectorOfChunks = splitVectorIntoChunks(numbersVector, vectorSize);
        
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
            v[i].join();
        }

        // Finalizando a contagem do tempo
        auto end = chrono::system_clock::now();

        chrono::duration<double> elapsed = end - start;

        totalTime = totalTime + elapsed.count();    

        return totalTime;  

}

float execute(int K, int N){

    int numberOfExecutions = 10;  
    double totalTimeForNumberOfExecutions = 0;
    vector<int> numbersVector = populateVector(N); //Populando o vetor de tamanho N com números no intervalo [-100,100] 

     for (int i = 1; i <= numberOfExecutions; i++)    {
         totalTimeForNumberOfExecutions+= adderWithSpinlocks(numbersVector, K, N);

     }
    printf("-----------------------------------------------------");        
    printf("Número de threads %i\n" , K);
    printf("Número de elementos no vetor inicial %i\n" , N);
    printf("Soma total %i\n" , totalSum);
    printf("Tempo médio para 10 iterações %f\n", totalTimeForNumberOfExecutions/numberOfExecutions);        
    printf("------------------------------------------------------");        

     
    return  totalTimeForNumberOfExecutions/numberOfExecutions;

}


int main(){
   
    vector<double> totalTime;
    vector<double> nThreads;

    for (int n = 10000000; n <=1000000000;){
        for (int k = 1; k <=  1;){
            
            totalTime.push_back(execute(k, n));
            nThreads.push_back(k);
            k = k*2;
        }
        n = n*10; 
    }
    
    RGBABitmapImageReference *imageReference = CreateRGBABitmapImageReference();
    
    StringReference *errorMessage = new StringReference();
    
    bool success = DrawScatterPlot(imageReference, 600, 400, &totalTime, &nThreads, errorMessage);

    if(success){
        vector<double> *pngdata = ConvertToPNG(imageReference->image);
        WriteToFile(pngdata, "example1.png");
        DeleteImage(imageReference->image);
	}else{
	   printf("Houve erro ao gerar gráfico");
	}

    return 0;
}