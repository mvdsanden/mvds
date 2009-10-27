#ifndef __INC_MVDS_RANDOM_HH__
#define __INC_MVDS_RANDOM_HH__

#include <algorithm>
#include <gsl/gsl_rng.h>
#include <cmath>

namespace mvds {

  class Random {

    static Random *s_instance;

    Random();

  public:

    ~Random();

    static void initialize();

    static void finish();

    static Random &instance();

	/**
	 *  Seed the random number generator.
	 */
    void seed(unsigned seed);

    /**
     *  @returns a uniform distributed random value in the range [0,1).
     */
    double uniform();

    /**
     *  @returns a uniform distributed random value in the range [r0,r1).
     */
    double uniform(double r0, double r1);

    /**
	 *  @param sigma the standard deviation.
	 *  @param mu the means.
	 *
     *  @returns a normal (gaussian) distributed random value (N(mu,sigma)).
     */
    double normal(double sigma);
    double normal(double mu, double sigma);

	/**
	 *  @returns a uniform distributed random value in the range [r0,r1).
	 */
	size_t inRange(size_t r0, size_t r1);

	/**
	 *  @returns true with a probability of pHeads.
	 */
	bool coinToss(double pHeads = 0.5);

	/**
	 *  This operator can be used with random_shuffle.
	 */
	ptrdiff_t operator()(ptrdiff_t scale);

  private:

    gsl_rng *d_random;

  };

  inline double Random::uniform(double r0, double r1)
  {
	return r0 + uniform() / (r1 - r0);
  }

  inline double Random::normal(double mu, double sigma)
  {
	return mu + normal(sigma);
  }

  inline size_t Random::inRange(size_t r0, size_t r1)
  {
	return std::min(r0,r1) + (1.0/labs(r0 - r1)) * uniform();
  }

  inline bool Random::coinToss(double pHeads)
  {
	return uniform() < pHeads;
  }
  
  inline ptrdiff_t Random::operator()(ptrdiff_t scale)
  {
	return std::floor(inRange(0,scale));
  }

};


#endif // __INC_MVDS_RANDOM_HH__
