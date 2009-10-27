#ifndef __INC_MVDS_THREADVALUE_HH__
#define __INC_MVDS_THREADVALUE_HH__

#include <pthread.h>

#include <stdexcept>

namespace mvds {

  template <class T>
  class ThreadValue {

	static void destruct(void *value);

  public:

	ThreadValue(bool finish = true);
	
	~ThreadValue();

	T *get();

	void set(T *ptr);

	T &operator*();

  private:

	pthread_key_t d_key;

  };

  template <class T>
  inline void ThreadValue<T>::destruct(void *value)
  {
	delete reinterpret_cast<T*>(value);
  }

  template <class T>
  inline ThreadValue<T>::ThreadValue(bool finish)
  {
	if (pthread_key_create(&d_key, (finish?destruct:0)))
	  throw std::runtime_error("Error creating thread specific key");
  }

  template <class T>	
  inline ThreadValue<T>::~ThreadValue()
  {
	if (pthread_key_delete(d_key))
	  throw std::runtime_error("Error deleting thread specific key");
  }

  template <class T>
  inline T *ThreadValue<T>::get()
  {
	return reinterpret_cast<T*>(pthread_getspecific(d_key));
  }

  template <class T>
  inline void ThreadValue<T>::set(T *ptr)
  {
	if (pthread_setspecific(d_key,ptr))
	  throw std::runtime_error("Error setting thread specific data");
  }

  template <class T>
  inline T &ThreadValue<T>::operator*()
  {
	return *get();
  }

};

#endif // __INC_MVDS_THREADVALUE_HH__
