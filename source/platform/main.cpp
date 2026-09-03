#include "platform.hpp"
#include "../game/world.hpp"
#include "../time/time.hpp"
#include "../utility.hpp"

#include <clocale>

#include <SDL2/SDL_main.h>

katha::World world = {};

int main(int argc, char** args)
{
	using namespace katha;
	Platform* platform = Platform::Get();
	platform->setForceExitCallback([](){
		world.clear();
	});

	char* locale = setlocale(LC_ALL, "");
	LogLine("locale: {s}", locale);

	Result result = platform->init(argc, args);
	if (!CheckResult(result, "platform::init"))
	{
		platform->clear();
		return static_cast<int>(result);
	}

	result = world.load();
	if (!CheckResult(result, "world::load"))
	{
		world.clear();
		platform->clear();
		return static_cast<int>(result);
	}

	bool running = true;
	const uint64_t start = Now();
	while (running)
	{
		platform->gles.renderWorld(world);

		uint64_t current = Now() - start;
		if (current > 3ULL * 1000 * 1000 * 1000)
		{
			running = false;
		}
	}

	world.clear();
	platform->clear();
	return 0;
}