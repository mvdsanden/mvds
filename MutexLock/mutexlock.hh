#ifndef __INC_MVDS_MUTEXLOCK_HH__
#define __INC_MVDS_MUTEXLOCK_HH__

#include <pthread.h>

namespace mvds {

  class MutexLock {

    friend class Condition;

    pthread_mutex_t d_mutex;

    void destroy();

    MutexLock(MutexLock const &); // NI
    MutexLock &operator=(MutexLock const &); // NI

  public:

    MutexLock();
    ~MutexLock();
    bool lock();
    bool unlock();
    bool tryLock();

  };

  inline void MutexLock::destroy()
  {
	pthread_mutex_destroy(&d_mutex);
  }

  inline MutexLock::MutexLock()
  {
	// Create a "fast" mutex.
	pthread_mutex_init(&d_mutex, 0);

	//d_mutex = PTHREAD_MUTEX_INITIALIZER;
  }

  inline MutexLock::~MutexLock() { destroy(); }

  inline bool MutexLock::lock()
  {
	return (!pthread_mutex_lock(&d_mutex));      
  }

  inline bool MutexLock::unlock()
  {
	return (!pthread_mutex_unlock(&d_mutex));
  }

  inline bool MutexLock::tryLock()
  {
	return (!pthread_mutex_trylock(&d_mutex));
  }   

};


#endif // __INC_MVDS_MUTEXLOCK_HH__
