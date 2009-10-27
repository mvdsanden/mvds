#include "random.ih"

void Random::seed(unsigned seed)
{
  gsl_rng_set(d_random,seed);
}
