#include "timeval.ih"

TimeVal TimeVal::now()
{
  TimeVal res;

  gettimeofday(res,0);

  return res;
}
