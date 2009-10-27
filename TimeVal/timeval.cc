#include "timeval.ih"

std::ostream &mvds::operator<<(std::ostream &os, const TimeVal &tv)
{
  os
    << tv.d_tv.tv_sec << " secs, " << tv.d_tv.tv_usec << " usecs";

  return os;
}
