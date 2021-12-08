#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <cmath>
#include <vector>
#include <iostream>
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



int main (int argc, char* argv[]) {

  auto fptr1 = f1;
  auto fptr2 = f2;
  auto fptr3 = f3;
  auto fptr4 = f4;

  std::vector<float (*)(float, int)> funct_list = {fptr1, fptr2, fptr3, fptr4};
  
  if (argc < 7) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads>"<<std::endl;
    return -1;
  }
  int functionId = atoi(argv[1]);
  float a = atof(argv[2]);
  float b = atof(argv[3]);
  int n = atoi(argv[4]);
  int intensity = atoi(argv[5]);
  int nbthreads = atoi(argv[6]);
  SeqLoop sl;
  auto start =std::chrono::steady_clock::now();
  
  float sum = 0.0;
  sl.parfor<float>(1, nbthreads, n,
            [&](float& tls) {
              tls = 0.0;
            },
            [&](int i, float& tls) {
              float x = (a + ((float)i + 0.5)) * ((float)(b-a)/n);
              tls += (float)funct_list[functionId - 1](x , intensity);
            },
            [&](float& tls) {
              sum += tls;
            }
        );
   float answer = ((float)(b-a)/n) *sum;
  auto stop = std::chrono::steady_clock::now();
  std::chrono::duration<double> time_elapsed = stop-start;

  std::cout<<answer<<" \n";
  std::cerr << time_elapsed.count();
  return 0;
}
