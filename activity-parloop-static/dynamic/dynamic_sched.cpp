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

float charToFloat(char* str){
  char *remaining;
  float number = strtof(str, &remaining);
}

float calcIntegral(float functionId, float a, float b, int n, int intensity, int nbthreads, int granularity){
  SeqLoop sl(n, nbthreads, granularity);

  float eq1 = (float)(b-a)/n;
  float answer = 0;
  std::vector<float> functionResponseVec;
  sl.parfor<float>(0, n, 1,
    [&](float& TLS) -> void{
           TLS = 0;
        },
        [&](int i, float&TLS)-> void{
           float eq2 = a + ((i +.5) * eq1);

       switch ((int) functionId){
        case 1:
          TLS += f1(eq2,intensity);
          break;
        case 2:
          TLS += f2(eq2,intensity);
          break;
        case 3:
          TLS += f3(eq2,intensity);
          break;
        case 4:
          TLS += f4(eq2,intensity);
          break;
          exit(0);
        }
          },
         [&](float &TLS) -> void{
      answer += TLS;
         }
  );
  
  return (eq1*answer);
}

int main (int argc, char* argv[]) {

  if (argc < 8) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <granularity>"<<std::endl;
    return -1;
  }
  float functionId = charToFloat(argv[1]);;
  float a = charToFloat(argv[2]);
  float b = charToFloat(argv[3]);
  int n = charToFloat(argv[4]);
  int intensity = charToFloat(argv[5]);
  int nbthreads = charToFloat(argv[6]);
  int granularity = charToFloat(argv[7]);
  auto start =std::chrono::steady_clock::now();
  float response = (float) calcIntegral(functionId, a, b, n, intensity, nbthreads, granularity);
  
    auto stop = std::chrono::steady_clock::now();
    std::chrono::duration<double> difference = stop-start;
    std::cout<< response<<std::endl;
    std::cerr << difference.count() << std::endl;

  return 0;
}
