#ifndef __INC_MVDS_CONDITION_HH_
#define __INC_MVDS_CONDITION_HH_

#include <pthread.h>

#include "../MutexLock/mutexlock.hh"
#include "../TimeSpec/timespec.hh"

namespace mvds {

  class Condition {

    pthread_cond_t d_cond;

    void destroy();

    Condition(Condition const &); // NI
    Condition &operator=(Condition const &); // NI

  public:

    Condition();

    ~Condition();

    int signal();

    int broadcast();

    int wait(MutexLock &mutex);

    int timedWait(MutexLock &mutex, TimeSpec const ts);

  };


  inline void Condition::destroy()
  {
    pthread_cond_destroy(&d_cond);
  }

  inline Condition::Condition()
  {
    pthread_cond_init(&d_cond, 0);

    //      d_cond = PTHREAD_COND_INITIALIZER;
  }

  inline Condition::~Condition() { destroy(); }

  inline int Condition::signal()
  {
    return pthread_cond_signal(&d_cond);
  }

  inline int Condition::broadcast()
  {
    return pthread_cond_broadcast(&d_cond);
  }

  inline int Condition::wait(MutexLock &mutex)
  {
    return pthread_cond_wait(&d_cond,&mutex.d_mutex);
  }

  inline int Condition::timedWait(MutexLock &mutex, TimeSpec const ts)
  {
    return pthread_cond_timedwait(&d_cond,&mutex.d_mutex,ts);
  }

};


#endif // __INC_MVDS_CONDITION_HH_
