#include "time.hpp"
#include "../core.hpp"

#include <ctime>

uint64_t katha::now()
{
	struct timespec ts = {};
	clock_gettime(CLOCK_MONOTONIC, &ts);
	const uint64_t ns = ts.tv_sec * 1000000000LL + ts.tv_nsec;
	return ns;
}