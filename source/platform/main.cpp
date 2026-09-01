#include "platform.hpp"
#include "../game/world.hpp"
#include "../time/time.hpp"
#include "../utility.hpp"

#include <clocale>

#include <SDL2/SDL_main.h>

katha::world_t world = {};

int main(int argc, char** args)
{
	using namespace katha;
	platform_t* platform = platform_t::get();
	platform->set_force_exit_callback([](){
		world.clear();
	});

	char* locale = setlocale(LC_ALL, "");
	log_line("locale: {s}", locale);

	result_e result = platform->init(argc, args);
	if (!check_result(result, "platform::init"))
	{
		platform->clear();
		return static_cast<int>(result);
	}

	result = world.load();
	if (!check_result(result, "world::load"))
	{
		world.clear();
		platform->clear();
		return static_cast<int>(result);
	}

	bool running = true;
	const uint64_t start = now();
	while (running)
	{
		platform->gl.render_world(world);

		uint64_t current = now() - start;
		if (current > 3ULL * 1000 * 1000 * 1000)
		{
			running = false;
		}
	}

	world.clear();
	platform->clear();
	return 0;
}