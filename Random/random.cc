#include "random.ih"

#include <fstream>

Random::Random()
{
  d_random = gsl_rng_alloc(gsl_rng_taus);

  // Seed the rng with the linux random number pool.
  ifstream urandom("/dev/urandom");
  unsigned value;
  urandom.read(reinterpret_cast<char*>(&value),sizeof(value));
  seed(value);
}

Random::~Random()
{
  gsl_rng_free(d_random);
}

