#ifndef __SEQ_LOOP_H
#define __SEQ_LOOP_H

#include <functional>
#include <vector>
#include <thread>
#include <iostream>
#include <mutex>
#include <fstream>
#include <string>

class SeqLoop {
private:
  int n;
  int nbthread;
  int granularity;
  int nbItr;
  std::mutex mutex1;
public:
  SeqLoop(int n, int nbthread, int granularity){
    this->n = n;
    this->nbthread = nbthread;
    this->granularity = granularity;
    this->nbItr = n / granularity;
  }
  void parfor (size_t beg, size_t end, size_t inc,
           std::function<void(int)> f) {
    for (size_t i=beg; i<end; i+= inc) {
      f(i);
    }
  }
  template<typename TLS>
  void parfor (size_t beg, size_t end, size_t increment,
           std::function<void(TLS&)> before,
           std::function<void(int, TLS&)> f,
           std::function<void(TLS&)> after
           ) {
    std::vector<TLS> tlsVec;
    std::vector<std::thread> threadVec;
    std::vector<bool> taskFlags(nbItr, false);
    int taskComplete = 0;
    for(int i=0; i<nbItr; i++){
      TLS tls;
      tlsVec.push_back(tls);
    }
    for(int k=0; k<nbItr; k++){
      before(tlsVec[k]);
    }
      
    for(int t=0; t<nbthread; t++){
        
      threadVec.push_back(std::thread([&](int &taskComplete) {
                 
    for(int i=0; i<nbItr; i++){
    bool taskStatus = false;
    auto glambda =  [&](std::vector<bool> &taskFlags) -> int{
        std::lock_guard<std::mutex> lg(mutex1);
        for (int j = 0; j < nbItr; j++) {
            if (taskFlags[j] == false) {
                taskFlags[j] = true;
                return j;
                      }
                  }
                  return -1;
        };
        int startIndex =  glambda(std::ref(taskFlags));
        int start = startIndex * granularity;
        if (startIndex == -1) break;
        taskPool(std::ref(start), tlsVec[startIndex], f, increment, std::ref(taskStatus));
            while (!taskStatus);
            }
            },std::ref(taskComplete)));
    }
    for (auto&thread :threadVec){
      thread.join();
    }
    for(auto& tls : tlsVec){
      after(tls);
    }
  }
  template<typename TLS>
  void taskPool(int &start, TLS &tls,std::function<void(int, TLS&) >f,int increment, bool &taskStatus){
    int end = start +granularity;
    for (size_t i=start; i<end; i+=increment){
        f(i, tls);
    }
    taskStatus = true;
  }
};
#endif
