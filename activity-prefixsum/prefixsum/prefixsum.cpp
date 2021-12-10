#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "omploop.hpp"
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
    
    
    if (argc < 3) {
        std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
        return -1;
    }
    
    int n = atoi(argv[1]);
    int threads = atoi(argv[2]);
    int* arr = new int [n];
    generatePrefixSumData (arr, n);
    
    int* pr = new int [n+1];
    std::vector<std::vector<int>> pr_parts(threads);
    //insert prefix sum code here
    auto start = std::chrono::system_clock::now();
    OmpLoop loop;
    loop.setNbThread(threads);
    loop.parfor<std::vector<int>>(
                                  0,threads, 1,
                                  [&](std::vector<int> &tls) {
                                  },
                                  [&](int i, std::vector<int> &tls)
                                  {
                                      tls.push_back(i);
                                      int start = i * (n / threads);
                                      int end = (i + 1) * (n / threads);
                                      if (i == threads -1)
                                      {
                                          end= n;
                                      }
                                      tls.push_back(arr[start++]);
                                      for (int i = start; i < end; i++)
                                      {
                                          tls.push_back(arr[i] + tls.back());
                                      }
                                  },
                                  [&](std::vector<int> &tls){
                                      pr_parts[tls.front()] = tls;
                                  }
                                  );
    loop.parfor<int> (
                      0,threads, 1,
                      [&](int & tls)
                      {
                          tls = 1;
                      },
                      [&](int i, int & tls)
                      {
                          int fix= 0;
                          for (int j = i; j > 0; j--)
                              fix += pr_parts[j - 1].back();
                          int start = i* (n / threads);
                          int end = (i+ 1) * (n/ threads);
                          if (i == threads -1){
                          } end = n;
                      }
                      for (int j = start; j < end; j++)
                      pr[j + 1] = pr_parts[i][tls++] + fix;
                      },
                      [&](int & tls){
                      });
    
    pr[0]= 0;
    checkPrefixSumResult(pr, n);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cerr << diff.count() << std::endl;
    delete[] arr;
    
    return 0;
}
