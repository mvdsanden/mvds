/*
 *  Socket Class
 *
 *  Author: Mart van de Sanden
 *
 *  License: GPL
 */

#ifndef __INC_MVDS_TIMEVAL_H_
#define __INC_MVDS_TIMEVAL_H_

#include <sys/time.h>
#include <time.h>
#include <iosfwd>

#include "../TimeSpec/timespec.hh"

namespace mvds {

  class TimeVal {

    friend std::ostream &operator<<(std::ostream &, const TimeVal &);

    struct timeval d_tv;

    void copy(TimeVal const &other)
    {
      d_tv.tv_sec  = other.d_tv.tv_sec;
      d_tv.tv_usec = other.d_tv.tv_usec;
    }

    void destroy()
    {
    }

  public:

    static TimeVal now();

    TimeVal();
    TimeVal(TimeVal const &other);
	TimeVal(TimeSpec const &ts);
    TimeVal(time_t sec, suseconds_t usec);
    TimeVal(struct timeval const &tv);
    ~TimeVal();

    TimeVal &operator=(TimeVal const &other);

    operator struct timeval &();
    operator struct timeval *();

	operator TimeSpec ();

    bool operator==(TimeVal const &other) const;
    bool operator!=(TimeVal const &other) const;
    bool operator>(TimeVal const &other) const;
    bool operator<(TimeVal const &other) const;
    bool operator>=(TimeVal const &other) const;
    bool operator<=(TimeVal const &other) const;
    TimeVal operator+(TimeVal const &other) const;
    TimeVal operator-(TimeVal const &other) const;
    double realSeconds() const;

	unsigned seconds() const;

	unsigned weeks() const;
	unsigned days() const;
	unsigned hours() const;
	unsigned min() const;
	unsigned sec() const;

	bool negative() const;
   

  };

  /*
  inline TimeVal const &max(TimeVal const &a, TimeVal const &b)
  {
	return (a > b?a:b);
  }

  inline TimeVal const &min(TimeVal const &a, TimeVal const &b)
  {
	return (a < b?a:b);
  }
  */

  inline TimeVal::TimeVal()
  {
    d_tv.tv_sec  = 0;
    d_tv.tv_usec = 0;
  }

  inline TimeVal::TimeVal(TimeVal const &other)
  {
    copy(other);
  }

  inline TimeVal::TimeVal(TimeSpec const &ts)
  {
	d_tv.tv_sec = ts.d_ts.tv_sec;
	d_tv.tv_usec = ts.d_ts.tv_nsec / 1000;
  }

  inline TimeVal::operator TimeSpec ()
  {
	return TimeSpec(d_tv.tv_sec,d_tv.tv_usec * 1000);
  }

  inline TimeVal::TimeVal(time_t sec, suseconds_t usec)
  {
    d_tv.tv_sec  = sec;
    d_tv.tv_usec = usec;
  }

  inline TimeVal::TimeVal(struct timeval const &tv)
  {
    d_tv.tv_sec  = tv.tv_sec;
    d_tv.tv_usec = tv.tv_usec;
  }

  inline TimeVal::~TimeVal()
  {
    destroy();
  }

  inline TimeVal &TimeVal::operator=(TimeVal const &other)
  {
    if (this != &other) {
      destroy();
      copy(other);
    }
    return *this;
  }

  inline TimeVal::operator struct timeval &()
  {
    return d_tv;
  }

  inline TimeVal::operator struct timeval *()
  {
    return &d_tv;
  }

  inline bool TimeVal::operator==(TimeVal const &other) const
  {
    return ((d_tv.tv_sec  == other.d_tv.tv_sec) &&
	    (d_tv.tv_usec == other.d_tv.tv_usec));
  }

  inline bool TimeVal::operator!=(TimeVal const &other) const
  {
    return ((d_tv.tv_sec  != other.d_tv.tv_sec) ||
	    (d_tv.tv_usec != other.d_tv.tv_usec));
  }

  inline bool TimeVal::operator>(TimeVal const &other) const
  {
    return ((d_tv.tv_sec > other.d_tv.tv_sec) ||
	    ((d_tv.tv_sec == other.d_tv.tv_sec) &&
	     (d_tv.tv_usec > other.d_tv.tv_usec)));
  }

  inline bool TimeVal::operator<(TimeVal const &other) const
  {
    return ((d_tv.tv_sec < other.d_tv.tv_sec) ||
	    ((d_tv.tv_sec == other.d_tv.tv_sec) &&
	     (d_tv.tv_usec < other.d_tv.tv_usec)));
  }

  inline bool TimeVal::operator>=(TimeVal const &other) const
  {
    return ((d_tv.tv_sec >= other.d_tv.tv_sec) ||
	    ((d_tv.tv_sec == other.d_tv.tv_sec) &&
	     (d_tv.tv_usec >= other.d_tv.tv_usec)));
  }

  inline bool TimeVal::operator<=(TimeVal const &other) const
  {
    return ((d_tv.tv_sec <= other.d_tv.tv_sec) ||
	    ((d_tv.tv_sec == other.d_tv.tv_sec) &&
	     (d_tv.tv_usec <= other.d_tv.tv_usec)));
  }

  inline TimeVal TimeVal::operator+(TimeVal const &other) const
  {
    return TimeVal((d_tv.tv_sec   + other.d_tv.tv_sec) +
		   ((d_tv.tv_usec + other.d_tv.tv_usec) / 1000000),
		   (d_tv.tv_usec  + other.d_tv.tv_usec) % 1000000);
  }

  inline TimeVal TimeVal::operator-(TimeVal const &other) const
  {
    struct timeval res;

    res.tv_sec = (d_tv.tv_sec - other.d_tv.tv_sec);

    if (d_tv.tv_usec < other.d_tv.tv_usec) {
      res.tv_sec -= (1 + (other.d_tv.tv_usec - d_tv.tv_usec) / 1000000);
      res.tv_usec = (1000000 - ((other.d_tv.tv_usec - d_tv.tv_usec) % 1000000));
    } else
      res.tv_usec = (d_tv.tv_usec - other.d_tv.tv_usec);

    return TimeVal(res);
  }

  inline double TimeVal::realSeconds() const
  {
    return d_tv.tv_sec + d_tv.tv_usec / 1000000.0;
  }
  
  inline unsigned TimeVal::weeks() const
  {
	return d_tv.tv_sec / (3600*24*7);
  }

  inline unsigned TimeVal::days() const
  {
	return (d_tv.tv_sec / (3600*24)) % 7;
  }

  inline unsigned TimeVal::hours() const
  {
	return (d_tv.tv_sec / 3600) % 24;
  }

  inline unsigned TimeVal::min() const
  {
	return (d_tv.tv_sec / 60) % 60;
  }

  inline unsigned TimeVal::sec() const
  {
	return d_tv.tv_sec % 60;
  }

  inline unsigned TimeVal::seconds() const
  {
	return d_tv.tv_sec;
  }

  inline bool TimeVal::negative() const
  {
	return (d_tv.tv_sec < 0 || (d_tv.tv_sec == 0 && d_tv.tv_usec < 0));
  }

  std::ostream &operator<<(std::ostream &os, const TimeVal &tv);

};

#endif //  __INC_MVDS_TIMEVAL_H_
