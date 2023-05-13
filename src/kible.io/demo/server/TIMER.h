// PLATFORMS: Linux, Windows, OSX (TODO)

#ifndef TIMER_H_
#define TIMER_H_

#include <thread>

#include "utils.h"

#ifdef __linux__
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#include <cstdlib>
#endif
#ifdef _WIN64
#include <windows.h>
#endif
#ifdef __APPLE__
#include <mach/clock.h>
#include <mach/mach.h>
#include <net/if.h>
#include <time.h>
#endif

struct TIMER {
#ifdef __linux__
	timeval start, end;
	long seconds, useconds;
#endif
#ifdef _WIN64
	double PCFreq;
	__int64 CounterStart;
#endif
#ifdef __APPLE__
	struct timespec start, end;
	long seconds, useconds;
#endif
};

bool Initialize_TIMER(TIMER*);
void Start_TIMER(TIMER*);
long Stop_TIMER(TIMER*);
void Delete_TIMER(TIMER*);

void Set_Interval(void (*func)(void*), unsigned int, void*);
void Set_Delay(void (*func)(void*), unsigned int, void*);
#endif /* TIMER_H_ */
