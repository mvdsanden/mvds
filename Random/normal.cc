#include "random.ih"

double Random::normal(double sigma)
{
  return gsl_ran_gaussian(d_random,sigma);
}
