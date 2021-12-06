#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <time.h>


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
    struct timespec start, finish;
    clock_gettime(CLOCK_MONOTONIC, &start);
  if (argc < 6) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }
    int functionid = 0, n = 0;
      
      float a = 0, b = 0, intensity = 0;
      float sum = 0, elapsed = 0;
      
      functionid = std::atoi(argv[1]);
      a = std::atoi(argv[2]);
      b = std::atoi(argv[3]);
      n = std::atoi(argv[4]);
      intensity = std::atoi(argv[5]);

      switch(functionid) {
          case 1:
            for(int i = 0; i < n; i++) {
              sum += f1(a + (i + 0.5)*((b-a) /n), intensity)*((b-a) /n);
            }
            break;
          case 2:
            for(int i = 0; i < n; i++) {
              sum += f2(a + (i + 0.5)*((b-a) /n), intensity)*((b-a) /n);
            }
            break;
          case 3:
            for(int i = 0; i < n; i++) {
              sum += f3(a + (i + 0.5)*((b-a) /n), intensity)*((b-a) /n);
            }
            break;
          case 4:
            for(int i = 0; i < n; i++) {
              sum += f4(a + (i + 0.5)*((b-a) /n), intensity)*((b-a) /n);
            }
            break;
      }
      
      std::cout << sum << std::endl;
      clock_gettime(CLOCK_MONOTONIC, &finish);
      elapsed = (finish.tv_nsec - start.tv_nsec);
      return 0;
    }
