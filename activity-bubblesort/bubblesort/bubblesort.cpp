#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include "omploop.hpp"
#include <pthread.h>
#include <omp.h>

#ifdef __cplusplus
extern "C" {
#endif

void generateMergeSortData (int* arr, size_t n);
void checkMergeSortResult (const int* arr, size_t n);

#ifdef __cplusplus
}
#endif

int main (int argc, char* argv[]) {
    auto start = std::chrono::system_clock::now();
    if (argc < 3) { std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
        return -1;
    }
    int n = atoi(argv[1]);
    int * arr = new int [n];
    generateMergeSortData (arr, n);
    OmpLoop oloop;
    oloop.setNbThread(atoi(argv[2]));
    for (int i= 0; i< n; i++){
        int first = i% 2;
        oloop.parfor<int>(
                          first, n - 1, 2,
                          [&](int (& tls)){
                              tls = 0;},
                          [&](int i, int (& tls)){
                              if(arr[i] > arr[i + 1])
                                  std::swap(arr[i] , arr[i + 1]);},
                          [&](int (& tls)){
                              tls++;
                          }
                          );
    }
    checkMergeSortResult (arr, n);
    // get runtime
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cerr << diff.count() << std::endl;
    
    delete[] arr;
    
    return 0;
}
