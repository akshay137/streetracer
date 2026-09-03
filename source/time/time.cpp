#include "time.hpp"
#include "../utility.hpp"

#if KATHA_LINUX == KATHA_PLATFORM
#include <ctime>
#elif KATHA_WINDOWS == KATHA_PLATFORM
#include <windows.h>
#endif

uint64_t katha::Now()
{
#if KATHA_LINUX == KATHA_PLATFORM
	struct timespec ts = {};
	clock_gettime(CLOCK_MONOTONIC, &ts);
	const uint64_t ns = ts.tv_sec * 1000000000LL + ts.tv_nsec;
	return ns;
#elif KATHA_WINDOWS == KATHA_PLATFORM
	static uint64_t frequency = [] {
		LARGE_INTEGER f;
		if (!QueryPerformanceFrequency(&f))
		{
			log_line("error-win: {s}", GetLastError());
		}

		return static_cast<uint64_t>(f.QuadPart);
	}();
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	
	const uint64_t ns = (t.QuadPart * 1000000000LL) / frequency;
	return ns;
#endif
}