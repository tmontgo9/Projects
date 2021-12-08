#ifndef __SEQ_LOOP_H
#define __SEQ_LOOP_H

#include <functional>

#include <thread>
#include <vector>

class SeqLoop {
private:
  size_t threads;
  std::vector<std::thread> worker_threads;

    
public:

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
    TLS tls;
    before(tls);
    for (size_t i=beg; i<end; i+= increment) {
      f(i, tls);
    }
    after(tls);
  }
  
  template<typename TLS>
  static void f_helper(size_t id_thread, std::function<void(int, TLS&)> f, TLS& tls, size_t part_size, size_t total_n, size_t total_threads, std::function<void(TLS&)> before) {
    before(tls);
    int thread_end = (id_thread + 1) * part_size;
    if (id_thread == total_threads) {
      thread_end = total_n;
    }
    
    for (size_t i = id_thread * part_size; i < thread_end; i++) {
      f(i, tls);
    }
  }
    template<typename TLS>
  void parfor_parallel (size_t beg, size_t end, size_t increment,
           std::function<void(TLS&)> before,
           std::function<void(int, TLS&)> f,
           std::function<void(TLS&)> after
           ) {
    std::vector<TLS> tlss (threads);
    
      
    
    size_t total_n = end - beg;
    size_t part_size = total_n / threads;
    
    for (size_t id=0; id < threads; id+= 1) {
      std::thread new_thread (f_helper<TLS>, id, f, std::ref(tlss[id]), part_size, total_n, threads, before);
      worker_threads.push_back(std::move(new_thread));
    }
    
    for (auto & t : worker_threads) {
      if (t.joinable()) {
          t.join();
      }
    }
    
    for (TLS tls : tlss) {
      after(tls);
    }
    
  }
  
  void set_thread_count(int count) {
    this -> threads = count;
  }
  
};

#endif
