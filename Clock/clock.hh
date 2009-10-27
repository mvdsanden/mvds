#ifndef __INC_MVDS_CLOCK_HH__
#define __INC_MVDS_CLOCK_HH__

#include <time.h>

#include "../TimeSpec/timespec.hh"

namespace mvds {

  class Clock {

  public:
	
	Clock(clockid_t clkId = CLOCK_REALTIME);

	~Clock();

	TimeSpec resolution() const;

	TimeSpec time() const;

  private:

	clockid_t d_id;

  };

  inline Clock::Clock(clockid_t clkId)
	: d_id(clkId)
  {
  }

  inline Clock::~Clock()
  {
  }

  inline TimeSpec Clock::resolution() const
  {
	TimeSpec t;
	clock_getres(d_id,t);
	return t;
  }

  inline TimeSpec Clock::time() const
  {
	TimeSpec t;
	clock_gettime(d_id,t);
	return t;
  }

};

#endif // __INC_MVDS_CLOCK_HH__
