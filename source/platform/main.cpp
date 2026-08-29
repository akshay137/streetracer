#include "platform.hpp"
#include "../graphics/gl/gl.hpp"
#include "../time/time.hpp"
#include "../type/transform.hpp"
#include "../type/action_map.hpp"
#include "../math/vector2.hpp"
#include "../math/quaternion.hpp"
#include "../math/matrix4.hpp"
#include "../utility.hpp"
#include "../constants.hpp"

#if KATHA_XR
#include "../graphics/xr/xr.hpp"
#endif

#include "../game/world.hpp"

#include <clocale>

#include <SDL2/SDL.h>

using namespace katha;

int main(int argc, char** args)
{
	char* locale = setlocale(LC_ALL, "");
	log_line("current locale: {s}", locale);
	log_line("engine: {s} {version}", ENGINE_NAME_UTF8, &ENGINE_VERSION);
	log_line("game: {s} {version}", GAME_NAME, &GAME_VERSION);

	float s = std::sin(radians(45));
	float c = std::cos(radians(45));
	quat_t q(transform_t::UP * s, c);
	vec3 r = rotate(q, transform_t::FORWARD);
	log_line("r: {v3}", r.array());
	// return 0;

	platform_t platform = {};
	result_e result = platform.init(argc, args);
	if (!check_result(result, "platform::init"))
	{
		platform.clear();
		return static_cast<int>(result);
	}

	graphics_i* gfx = platform.get_graphics();

	world_t world = {};
	if (!check_result(world.load(gfx), "world::load"))
	{
		log_line("error: failed to create gl_scene");
		world.clear(gfx);
		platform.clear();
		return 0;
	}

	bool running = true;
	uint64_t last = 0;
	while (running)
	{
		// frame::begin
		const uint64_t start = now();
		float delta = 0.016f;
		if (last)
		{
			delta = last / static_cast<double>(1e9);
		}

		result = platform.poll_events();
		if (result_e::request_exit == result)
		{
			running = false;
			break;
		}

		// frame::logic_tick
		action_map_t action_map = platform.get_action_map();
		world.update(action_map, delta);

		// frame::render
		if (platform.config.enabled(feature_e::vr))
		{
#if KATHA_XR
			xr->poll_events();
			xr_t::frame_t xr_frame = {};
			result = xr->begin_frame(&xr_frame);
			if (result_e::request_skip == result)
			{
				continue;
			}
			if (!check_result(result, "xr::begin_frame"))
			{
				running = false;
				break;
			}

			gfx->render(
				world,
				render_mode_e::stereo,
				xr_frame.get_transform(xr_t::EYE_LEFT),
				xr_frame.framebuffer_left(),
				xr_frame.get_transform(xr_t::EYE_RIGHT),
				xr_frame.framebuffer_right()
			);

			result = xr->end_frame(xr_frame);
			if (!check_result(result, "xr::end_frame"))
			{
				running = false;
				break;
			}
#endif
		}
		else
		{
			uvec2 size = platform.get_drawable_size();
			gfx->render(world);
		}
		
		// frame::present
		// shows left eye in xr mode
		gfx->present_to_screen();

		// frame::end
		last = now() - start;
		if (platform.config.enabled(feature_e::log_frame_time))
		{
			log_line("frame_time: {td}, delta: {f}", last, delta);
		}
	}

	world.clear(gfx);
	platform.clear();
	return 0;
}