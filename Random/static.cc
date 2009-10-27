#include "random.ih"

#include <cassert>

Random *Random::s_instance = 0;

void Random::initialize()
{
  if (s_instance)
    throw runtime_error("Random already initialized");

  s_instance = new Random;
}

Random &Random::instance()
{
  return *s_instance;
}

void Random::finish()
{
  if (s_instance)
	delete s_instance;
}
