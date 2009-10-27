/*
 *  Author: Mart van de Sanden
 *
 *  License: GPL
 */

#ifndef __INC_MVDS_TIMESPEC_H_
#define __INC_MVDS_TIMESPEC_H_

#include <sys/time.h>
#include <time.h>
#include <iosfwd>

namespace mvds {

  class TimeSpec {

    friend std::ostream &operator<<(std::ostream &, const mvds::TimeSpec &);
	friend class TimeVal;

    void copy(TimeSpec const &other)
    {
      d_ts.tv_sec  = other.d_ts.tv_sec;
      d_ts.tv_nsec = other.d_ts.tv_nsec;
    }

    void destroy()
    {
    }

  public:

    static TimeSpec now();

    TimeSpec();
    TimeSpec(TimeSpec const &other);
    TimeSpec(time_t sec, long nsec);
    TimeSpec(struct timespec const &ts);
    ~TimeSpec();

    TimeSpec &operator=(TimeSpec const &other);

    operator struct timespec &();
    operator struct timespec *();

    operator struct timespec const &() const;
    operator struct timespec const *() const;

    bool operator==(TimeSpec const &other) const;
    bool operator!=(TimeSpec const &other) const;
    bool operator>(TimeSpec const &other) const;
    bool operator<(TimeSpec const &other) const;
    bool operator>=(TimeSpec const &other) const;
    bool operator<=(TimeSpec const &other) const;
    TimeSpec operator+(TimeSpec const &other) const;
    TimeSpec operator-(TimeSpec const &other) const;
    double getRealSeconds() const;

  private:

    struct timespec d_ts;

  };


  inline TimeSpec::TimeSpec()
  {
    d_ts.tv_sec  = 0;
    d_ts.tv_nsec = 0;
  }

  inline TimeSpec::TimeSpec(TimeSpec const &other)
  {
    copy(other);
  }

  inline TimeSpec::TimeSpec(time_t sec, long nsec)
  {
    d_ts.tv_sec  = sec;
    d_ts.tv_nsec = nsec;
  }

  inline TimeSpec::TimeSpec(struct timespec const &ts)
  {
    d_ts.tv_sec  = ts.tv_sec;
    d_ts.tv_nsec = ts.tv_nsec;
  }

  inline TimeSpec::~TimeSpec()
  {
    destroy();
  }

  inline TimeSpec &TimeSpec::operator=(TimeSpec const &other)
  {
    if (this != &other) {
      destroy();
      copy(other);
    }
    return *this;
  }

  inline TimeSpec::operator struct timespec &()
  {
    return d_ts;
  }

  inline TimeSpec::operator struct timespec *()
  {
    return &d_ts;
  }

  inline TimeSpec::operator struct timespec const &() const
  {
    return d_ts;
  }

  inline TimeSpec::operator struct timespec const *() const
  {
    return &d_ts;
  }


  inline bool TimeSpec::operator==(TimeSpec const &other) const
  {
    return ((d_ts.tv_sec  == other.d_ts.tv_sec) &&
	    (d_ts.tv_nsec == other.d_ts.tv_nsec));
  }

  inline bool TimeSpec::operator!=(TimeSpec const &other) const
  {
    return ((d_ts.tv_sec  != other.d_ts.tv_sec) ||
	    (d_ts.tv_nsec != other.d_ts.tv_nsec));
  }

  inline bool TimeSpec::operator>(TimeSpec const &other) const
  {
    return ((d_ts.tv_sec > other.d_ts.tv_sec) ||
	    ((d_ts.tv_sec == other.d_ts.tv_sec) &&
	     (d_ts.tv_nsec > other.d_ts.tv_nsec)));
  }

  inline bool TimeSpec::operator<(TimeSpec const &other) const
  {
    return ((d_ts.tv_sec < other.d_ts.tv_sec) ||
	    ((d_ts.tv_sec == other.d_ts.tv_sec) &&
	     (d_ts.tv_nsec < other.d_ts.tv_nsec)));
  }

  inline bool TimeSpec::operator>=(TimeSpec const &other) const
  {
    return ((d_ts.tv_sec >= other.d_ts.tv_sec) ||
	    ((d_ts.tv_sec == other.d_ts.tv_sec) &&
	     (d_ts.tv_nsec >= other.d_ts.tv_nsec)));
  }

  inline bool TimeSpec::operator<=(TimeSpec const &other) const
  {
    return ((d_ts.tv_sec <= other.d_ts.tv_sec) ||
	    ((d_ts.tv_sec == other.d_ts.tv_sec) &&
	     (d_ts.tv_nsec <= other.d_ts.tv_nsec)));
  }

  inline TimeSpec TimeSpec::operator+(TimeSpec const &other) const
  {
    return TimeSpec((d_ts.tv_sec   + other.d_ts.tv_sec) +
		   ((d_ts.tv_nsec + other.d_ts.tv_nsec) / 1000000000),
		   (d_ts.tv_nsec  + other.d_ts.tv_nsec) % 1000000000);
  }

  inline TimeSpec TimeSpec::operator-(TimeSpec const &other) const
  {
    struct timespec res;

    res.tv_sec  = (d_ts.tv_sec - other.d_ts.tv_sec);
	res.tv_nsec = (d_ts.tv_nsec - other.d_ts.tv_nsec);

	if (res.tv_nsec < 0) {
	  --res.tv_sec;
	  res.tv_nsec += 1000000000;
	}

    return TimeSpec(res);
  }

  inline double TimeSpec::getRealSeconds() const
  {
    return d_ts.tv_sec + d_ts.tv_nsec / 1000000000.0;
  }

  std::ostream &operator<<(std::ostream &os, const TimeSpec &tv);

};

#endif //  __INC_MVDS_TIMESPEC_H_
