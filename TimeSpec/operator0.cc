#include "timespec.ih"

std::ostream &mvds::operator<<(std::ostream &os, const mvds::TimeSpec &tv)
{

  if (tv.d_ts.tv_sec != 0)
	os << tv.d_ts.tv_sec << "s";

  return os << tv.d_ts.tv_nsec << "ns";

}
