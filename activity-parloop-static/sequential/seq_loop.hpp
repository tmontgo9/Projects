#ifndef __SEQ_LOOP_H
#define __SEQ_LOOP_H

#include <iostream>
#include <functional>
#include <thread>
#include <vector>

class SeqLoop {
public:
  int n, nbthreads;

  SeqLoop(int n, int nbthreads){
    this -> n =n;
    this -> nbthreads =nbthreads;
  }
  void parfor (size_t beg, size_t end, size_t increment1,
           std::function<void(int)> f) {
    for (size_t i=beg; i<end; i+= increment1) {
      f(i);
    }
  }
  template<typename TLS>
  void parfor (size_t beg, size_t end, size_t increment,
           std::function<void(TLS&)> before,
           std::function<void(int, TLS&)> f,
           std::function<void(TLS&)> after
           ) {
    std::vector<TLS> functionvector;
    std::vector<std::thread> mythreads;

    for(int i=0;i<nbthreads;i++) {
    TLS tls;
    functionvector.push_back(tls);
      }
    int a = (n/nbthreads);
    for(int t=0; t<nbthreads; t++) {
    before(functionvector[t]);

    mythreads.push_back(std::thread([&](size_t beg, size_t end, int increment, TLS &tls){
          for (size_t i=beg;i<end;i+=increment){
          f(i,tls);
        }},
        t *a, (nbthreads - 1) == t ? n: (t * a) + a, 1, std::ref(functionvector[t]))
      );
      }

    for(auto& thread : mythreads){
    thread.join();
      }
    for(auto& b : functionvector){
    after(b);
      }
  }

};

#endif
