#pragma once

class AgRandom
{
	int _seed;
	long s;

public:
	int GetSeed();
	void SeedRNG(unsigned int seed);
	int GetRandomInt();
};
