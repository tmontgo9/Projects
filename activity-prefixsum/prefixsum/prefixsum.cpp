#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include "omploop.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <chrono>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif
void generatePrefixSumData (int* arr, size_t n);
void checkPrefixSumResult (int* arr, size_t n);
#ifdef __cplusplus
}
#endif

int main (int argc, char* argv[]) {
    auto begin = std::chrono::system_clock::now();
    if (argc < 3) {
        std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
        return -1;
    }
    int n= atoi(argv[1]);
    int threads= atoi(argv[2]);
    int * arr= new int [n];
    generatePrefixSumData (arr, n);
    int * pr= new int [n+1];
    std::vector<std::vector<int>> pr_parts(threads);
    
    //insert prefix sum code here
    OmpLoop oLoop;
    oLoop.setNbThread(threads);
    oLoop.parfor<std::vector<int>>(
                                0,threads,1,
                                [&](std::vector<int> &tls) {
                                },
                                [&](int i,std::vector<int> &tls){
                                    
                                    tls.push_back(i);
                                    
                                    int begin = i * (n / threads);
                                    int end = (i + 1) * (n / threads);
                                    
                                    if (i == threads -1)
                                    {
                                        end= n;
                                    }
                                    tls.push_back(arr[begin++]);
                                    for (int i = begin; i < end; i++)
                                        tls.push_back(arr[i] + tls.back());
                                },
                                [&](std::vector<int> &tls){
                                    pr_parts[tls.front()] = tls;
                                });
    oLoop.parfor<int> (
                    0, threads, 1,
                    [&](int & tls){
                        tls = 1;
                    },
                    [&](int i, int & tls){
                        int a1 = 0;
                        for (int j = i; j > 0; j--)
                            a1 += pr_parts[j - 1].back();
                        int begin = i * (n / threads);
                        //pr_parts
                        int end = (i + 1) * (n / threads);
                        if (i == threads -1)
                        {
                            end = n;
                            
                        }
                        for (int j = begin; j < end; j++)
                            pr[j + 1] = pr_parts[i][tls++]+ a1 ;
                    },
                    [&](int& tls){
                    });
    pr[0] = 0;
    checkPrefixSumResult(pr, n);
    
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - begin;
    std::cerr << diff.count() << std::endl;
    
    delete[] arr;
    
    return 0;
}
