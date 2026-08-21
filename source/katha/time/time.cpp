#include "time.hpp"
#include "../core.hpp"

#if KATHA_LINUX == KATHA_PLATFORM
#include <ctime>
#elif KATHA_WINDOWS == KATHA_PLATFORM
#include <windows.h>
#endif

uint64_t katha::now()
{
#if KATHA_LINUX == KATHA_PLATFORM
	struct timespec ts = {};
	clock_gettime(CLOCK_MONOTONIC, &ts);
	const uint64_t ns = ts.tv_sec * 1000000000LL + ts.tv_nsec;
	return ns;
#elif KATHA_WINDOWS == KATHA_PLATFORM
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	const int64_t ns = t.QuadPart;
	return static_cast<uint64_t>(ns);
#endif
}