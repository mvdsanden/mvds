#ifndef __INC_MVDS_THREAD_HH_
#define __INC_MVDS_THREAD_HH_

#include <pthread.h>
#include <stdexcept>

#include <iostream>

namespace mvds {

  /**
   *  TODO: Do something with cleanup handlers!
   *
   *
   */
  class Thread {

    void destroy();

    static void *executeThread(void *object);

    Thread(Thread const &); // NI
    Thread &operator=(Thread const &); // NI

  protected:

    void exit(int res);

  public:

    Thread();

    virtual ~Thread();

    virtual int main() = 0;

    void execute();

    int join();

    int id() const;

  private:

    pthread_t d_thread;

    int d_id;

  };



  inline void Thread::destroy()
  {
    //    exit(0);
  }

  inline void *Thread::executeThread(void *object)
  {
    return reinterpret_cast<void *>(reinterpret_cast<Thread *>(object)->main());
  }

  inline void Thread::exit(int res)
  {
    pthread_exit(reinterpret_cast<void *>(&res));
  }

  inline Thread::Thread()
  {
  }

  inline Thread::~Thread() { destroy(); }

  inline void Thread::execute()
  {
    int res = pthread_create(&d_thread,0,
			     executeThread,
			     reinterpret_cast<void*>(this));

    if (res)
      throw std::runtime_error("error initializing thread");

    d_id = d_thread;
  }

  inline int Thread::join()
  {
    int res = 0;
    pthread_join(d_thread,reinterpret_cast<void **>(&res));
    return res;
  }

  inline int Thread::id() const
  {
    return d_id;
  }

};


#endif // __INC_MVDS_THREAD_HH_
