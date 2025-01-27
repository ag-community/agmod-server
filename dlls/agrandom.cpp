#include "agrandom.h"


// This is just a LGC with the same parameters that MSVC uses
// See: https://en.wikipedia.org/wiki/Linear_congruential_generator#Parameters_in_common_use
// We'll use this instead of srand() and rand() from the standard library, because we need the
// sequences to be the same in every system, otherwise in Linux the sequence is different due to,
// for example, glibc having a different implementation

int AgRandom::GetSeed()
{
	return _seed;
}

void AgRandom::SeedRNG(unsigned int seed)
{
	_seed = seed;
	s = (long)seed;
}

int AgRandom::GetRandomInt()
{
	return (((s = s * 214013L + 2531011L) >> 16) & 0x7fff);
}
