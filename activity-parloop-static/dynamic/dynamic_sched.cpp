#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <cmath>
#include <thread>
#include <vector>
#include "seq_loop.hpp"


#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif
using namespace std::chrono;

float get_function_value(int f,float x, int intensity) {
    switch(f) {
      case 1:
          return f1(x, intensity);
          break;
      case 2:
          return f2(x, intensity);
          break;
      case 3:
          return f3(x, intensity);
          break;
      case 4:
          return f4(x, intensity);
          break;
  }
  std::cout << "error";
  return 0;
}

void worker_func(int functionid,int low,int n,float start,float *tempV,int thread_start,int intensity,int thread_end) {
    float temp1 = 0.0f;
    SeqLoop loop1;
    loop1.parfor(thread_start, thread_end,1, {
      [&](int i) -> void{
          float x_value = low + (i + 0.5f) * start;
        temp1+= get_function_value(functionid, x_value, intensity);
        }
    }
  );
  *tempV =temp1;
}

int main (int argc, char* argv[]) {

  if (argc < 7) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads>"<<std::endl;
    return -1;
  }
  
  int fuctionID  = atoi(argv[1]);
  int low = atoi(argv[2]);
  int high = atoi(argv[3]);
  int n = atoi(argv[4]);
  int intensity  = atoi(argv[5]);
  int nbthreads  = atoi(argv[6]);
  auto clockStart = system_clock::now();
  float result = 0;
  float start = (high - low) / static_cast<float>(n);
  float temp1 = 0.0f;
  std::vector<std::thread> threads;
  float tempV[nbthreads] = {0};
  int loop_i =n/nbthreads;
  
  for (int i = 0; i < nbthreads; i++){
      int thread_start = (i) * loop_i;
      int thread_end  = ((i + 1) * loop_i) - 1;
      if (i == nbthreads - 1) {
          thread_end++;
      }
      std::thread minion_thread (worker_func, fuctionID, low, n, start, intensity, &tempV[i], thread_start, thread_end);
      threads.push_back(std::move(minion_thread));
  }
  for (auto & t : threads) {
      if (t.joinable()) {
          t.join();
    }
  }
  for (int i = 0; i < nbthreads; i++) {
    temp1 = temp1 + tempV[i];
  }
  result = start * temp1;
  std::cout << result;
  auto clockEnd = system_clock::now();
    
  std::chrono::duration<double> diff = clockEnd - clockStart;
  std::cerr << diff.count();
  
  return 0;
}
