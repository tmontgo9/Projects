#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <vector>
#include <array>
#include "../sequential/seq_loop.hpp"
#ifdef __cplusplus
extern "C" {
#endif
//simplified, fix later
float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif
typedef float (*ptr) (float,int);
ptr getFunction(int f) {
  switch (f) {
    case 1:
      return &f1;
    case 2:
      return &f2;
    case 3:
      return &f3;
    default:
      return &f4;
  }
}
int main (int argc, char* argv[]) {

  auto start = std::chrono::system_clock::now();
  if (argc < 7) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads>"<<std::endl;
    return -1;
  }
  std::array<float, 6> point;
  SeqLoop s1;
  for (int i = 0; i < 6; i++) {
    point[i] = atoi(argv[i + 1]);
  }

  float ans = 0;
  int func = point[0];
  int a = point[1];
  int b = point[2];
  int n = point[3];
  int intensity = point[4];
  int threads = point[5];

  float (*ptr)(float, int) = getFunction(func);
  float t1 =  (b - a) / float (n);
    s1.setNBThread(threads);
    s1.parfor<float>(
    0, n, 1,
    [&](float (&tls)) -> void { tls =0;
    },
    [&](int i, float (&tls)) -> void { tls += (*ptr)(a + ((i +.5) * t1), intensity);
    },
    [&](float (&tls)) -> void {
      ans +=B;
    }
  );
  
  ans = ans *t1;
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> diff =end -start;
  std::cout << ans << std::endl;
  std::cerr << diff.count() << std::endl;

  return 0;
}
