#include "random.ih"

double Random::uniform()
{
  return gsl_rng_uniform(d_random);
}
