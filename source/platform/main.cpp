#include "platform.hpp"
#include "gl/gl.hpp"
#include "../katha/time/time.hpp"
#include "../katha/physics/vertex.hpp"
#include "../katha/physics/transform.hpp"
#include "../katha/math/vector2.hpp"
#include "../katha/math/quaternion.hpp"
#include "../katha/math/matrix4.hpp"

#if KATHA_XR
#include "xr/xr.hpp"
#endif

#include "../game/action_map.hpp"
#include "../game/highway.hpp"

#include <clocale>

#include <SDL2/SDL.h>

using namespace katha;


int main(int argc, char** args)
{
	char* locale = setlocale(LC_ALL, "");
	log_line("current locale: {s}", locale);
	log_line("unicode test: {s}", ENGINE_NAME_UTF8);

	platform_t platform = {};
	result_e result = platform.init(argc, args);
	if (!check_result(result, "platform::init"))
	{
		platform.clear();
		return static_cast<int>(result);
	}

	graphics_i* gfx = platform.get_graphics();

	highway_t highway = {};
	if (!check_result(highway.load(gfx), "highway::load"))
	{
		log_line("error: failed to create gl_scene");
		highway.clear(gfx);
		platform.clear();
		return 0;
	}

	transform_t camera = { .orientation = quat_t::identity(), .position = vec3(0, 3, 9) };

	bool running = true;
	uint64_t last = 0;
	while (running)
	{
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

		const gamepad_t& gp = platform.current_input_state.gamepad;
		action_map_t action_map = platform.get_action_map();

		highway.update(action_map, delta);

		camera.position.x = lerp(camera.position.x, highway.player.x, 5 * delta);
		camera = camera.look_at(highway.player + vec3(0, 3, 0));

		if (platform.config.enable_xr)
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

			// render begin
			gfx->render(
				highway,
				render_mode_e::stereo,
				camera.offset_by(xr_frame.get_transform(xr_t::EYE_LEFT)),
				xr_frame.framebuffer_left(),
				camera.offset_by(xr_frame.get_transform(xr_t::EYE_RIGHT)),
				xr_frame.framebuffer_right()
			);
			// render end

			result = xr->end_frame(xr_frame);
			if (!check_result(result, "xr::end_frame"))
			{
				running = false;
				break;
			}

			// show left eye on window
			gfx->present_to_screen();
#endif
		}
		else
		{
			uvec2 size = platform.get_drawable_size();
			gfx->render(camera, highway);
			gfx->present_to_screen();
		}

		last = now() - start;
		if (platform.config.log_frame_time)
		{
			log_line("frame_time: {td}, delta: {f}", last, delta);
		}
	}

	highway.clear(gfx);
	platform.clear();
	return 0;
}