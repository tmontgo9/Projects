#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>

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

float numericalintegration(int functionid, float a, float b, int n, int intensity)
{
  float ans = (b-a)/n;
  float total = 0;
  float area = 0;
  if(functionid ==  1){

  for(int i = 0; i < n; i++){
      area = ans * f1(a + (i+.5)*ans, intensity);
      total = area+total;
    }
  }
  else if(functionid == 2){

  for(int i = 0; i < n; i++){
      area = ans *  f2(a + (i+.5)*ans, intensity);
      total = area+total;
    }
  }
  else if(functionid == 3){

  for(int i = 0; i < n; i++){
       area = ans * f3(a + (i+.5)*ans, intensity);
       total = area+total;
    }
  }
  else if (functionid == 4){

  for(int i = 0; i < n; i++) {
      area = ans * f4(a + (i+.5)*ans, intensity);
      total = area+total;
    }
  }
  else{
    return 1.0;
  }
  return total;
}
int main (int argc, char* argv[]) {
  using namespace std::chrono;
 if (argc < 6) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }
 double integral = 0;
 
  int functionid = atoi(argv[1]);
  float a = atof(argv[2]);
  float b = atof(argv[3]);
  int n = atoi(argv[4]);
  int intensity = atoi(argv[5]);

  auto start = std::chrono::steady_clock::now();
  integral = numericalintegration(functionid, a, b, n, intensity);
  auto end = std::chrono::steady_clock::now();
  auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
  float time = microseconds/1000000.0;
  std::cout << integral << std::endl;
  std::cerr << std::to_string(time) << " seconds." << std::endl;
  return 1;
}
