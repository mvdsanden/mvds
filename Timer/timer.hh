#ifndef __INC_MVDS_TIMER_HH__
#define __INC_MVDS_TIMER_HH__

#include "../Clock/clock.hh"

namespace mvds {

  class Timer {

  public:

	Timer();

	~Timer();

	void tick();

	TimeSpec tock() const;

	TimeSpec resolution() const;

  private:

	Clock    d_clock;
	TimeSpec d_tick;

  };

  inline Timer::Timer()
  {
  }

  inline Timer::~Timer()
  {
  }

  inline void Timer::tick()
  {
	d_tick = d_clock.time();
  }

  inline TimeSpec Timer::tock() const
  {
	return d_clock.time() - d_tick;
  }

  inline TimeSpec Timer::resolution() const
  {
	return d_clock.resolution();
  }

};

#endif // __INC_MVDS_TIMER_HH__
