#include <iostream>
#include <thread>
using namespace std;
void print(int x){
cout <<"Hello! I am minion " << x << endl;
}
int main (int argc, char** argv) {
  if (argc < 2) {
    std::cerr<<"usage: "<<argv[0]<<" <nbminions>\n";
    return -1;
  }
  //Your code goes here
    int n = atoi(argv[1]);
    std::thread mythreads[n];
    for (int i = 0; i<n;i++){
        mythreads[i] =std::thread(print,i+1);
    }
    for (int i=0;i<n;i++){
        mythreads[i].join();
    }
  cout<<"Hello Minions! I am the Overlord!"<<endl;
  return 0;
}
