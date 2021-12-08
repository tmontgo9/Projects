#include <iostream>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

float charConversion(char* str){
  char * remaining;
  float number = strtof(str, &remaining);
  if (str == remaining){
    std::cerr<<"error"<<std::endl;
    exit(0);
  }
  return number;
}

float integralConversion(float functionId, float a, float b, int n, int intensity, int nbthreads, int granularity){
  SeqLoop sl(n, nbthreads, granularity);

  float eq1 = (float)(b-a)/n; float funcAns = 0;
  std::vector<float> functionResponseVec;
  sl.parfor<float>(0, n, 1,
    [&](float& tls) -> void{
           tls = 0;
        },
        [&](int i, float& tls) -> void{
           float eq2 = a + ((i + 0.5) * eq1);
       switch ((int) functionId){
        case 1:
          tls += f1(eq2,intensity);
          break;
        case 2:
          tls += f2(eq2,intensity);
          break;
        case 3:
          tls += f3(eq2,intensity);
          break;
        case 4:
          tls += f4(eq2,intensity);
          break;
        default:
          std::cerr<<"error"<<std::endl;
          exit(0);
        }
          },
         [&](float &tls) -> void{
      funcAns += tls;
         }
  );
  
  return (eq1*funcAns);
}

int main (int argc, char* argv[]){

  if (argc < 8) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <granularity>"<<std::endl;
    return -1;
  }
  float functionId = charConversion(argv[1]);
  float a = charConversion(argv[2]);
  float b = charConversion(argv[3]);
  int n = charConversion(argv[4]);
  int intensity = charConversion(argv[5]);
  int nbthreads = charConversion(argv[6]);
  int granularity = charConversion(argv[7]);
  auto start =std::chrono::steady_clock::now();
  float response = (float) integralConversion(functionId, a, b, n, intensity, nbthreads, granularity);
  auto stop = std::chrono::steady_clock::now();
  
    std::chrono::duration<double> difference = stop-start;
    std::cout<< response<<std::endl;
    std::cerr << difference.count() << std::endl;

  return 0;
}
