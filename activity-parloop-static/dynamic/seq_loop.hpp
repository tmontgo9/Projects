#ifndef __SEQ_LOOP_H
#define __SEQ_LOOP_H

#include <functional>
//simplied, fix later, working
class SeqLoop {
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
  
};

#endif
