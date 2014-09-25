#ifndef TIMING_H_
#define TIMING_H_

#include <sys/time.h>
#include <iostream>

using namespace std;

class Timing
{
	unsigned long secs, usecs;
	struct timeval tt;
	clock_t start;
public:
	Timing();
	virtual ~Timing();

	void startClock();
	unsigned long elapsed();
	unsigned long stopClock();
};

#endif /*TIMING_H_*/
