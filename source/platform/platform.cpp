#include "platform.hpp"
#include "../graphics/gl/gl.hpp"
#include "../graphics/vulkan/vulkan_context.hpp"
#include "../constants.hpp"
#include "../math/utility.hpp"
#include "../utility.hpp"

#if KATHA_XR
#include "../graphics/xr/xr.hpp"
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>
#include <SDL2/SDL_vulkan.h>

katha::result_e katha::platform_t::init(int argc, char** args)
{
	config.parse_command_line(argc, args);
	config.log();

	SDL_version version = {};
	SDL_VERSION(&version);
	log_line("sdl: compiled version: {u}.{u}.{u}",
		version.major, version.minor, version.patch
	);

	SDL_GetVersion(&version);
	log_line("sdl: loaded version: {u}.{u}.{u}",
		version.major, version.minor, version.patch
	);

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
	{
		log_line("error: SDL_Init {s}", SDL_GetError());
		return result_e::error_sdl;
	}

	result_e result = query_displays();
	if (!check_result(result, "query_displays"))
	{
		return result;
	}
	result = query_controllers();
	if (!check_result(result, "query_controllers"))
	{
		return result;
	}

	if (graphics_api_e::vulkan == config.graphics_api)
	{
		result = init_with_vulkan();
		if (result_e::success != result)
		{
			config.graphics_api = graphics_api_e::gl;
			log_line("vulkan is not available, falling back to opengl");
		}
	}

	if (graphics_api_e::gl == config.graphics_api)
	{
		result = init_with_gl();
		if (!check_result(result, "init_with_gles"))
		{
			return result;
		}
	}

	return result_e::success;
}

void katha::platform_t::clear()
{
	log_line("platform::clear()");

	if (gamepads)
	{
		for (int i = 0; i < gamepad_count; i++)
		{
			SDL_GameControllerClose(gamepads[i]);
		}
		release(gamepads);
		gamepads = nullptr;
	}

#if KATHA_XR
	if (config.enabled(feature_e::vr))
	{
		xr->clear();
	}
#endif

	if (graphics_api_e::gl == config.graphics_api)
	{
		clear_gl();
	}
	else if (graphics_api_e::vulkan == config.graphics_api)
	{
		clear_vulkan();
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
}

katha::graphics_i* katha::platform_t::get_graphics()
{
	switch (config.graphics_api)
	{
		case graphics_api_e::gl: return gl;
		// case graphics_api_e::vulkan: return vulkan;
	}

	return nullptr;
}

katha::result_e katha::platform_t::query_displays()
{
	int32_t available_displays = SDL_GetNumVideoDisplays();
	if (available_displays < 0)
	{
		log_line("error: SDL_GetNumVideoDisplays {s}", SDL_GetError());
		return result_e::error_sdl;
	}
	if (config.preferred_display_index >= available_displays)
	{
		config.preferred_display_index = 0;
	}

	display_index = clamp<int32_t>(
		config.preferred_display_index,
		0, available_displays
	);
	log_line("sdl: total displays {i}", available_displays);

	SDL_Rect bounds = {};
	if (SDL_GetDisplayBounds(display_index, &bounds) < 0)
	{
		log_line("error: SDL_GetDisplayBounds {s}", SDL_GetError());
		return result_e::error_sdl;
	}

	display_size.x = bounds.w;
	display_size.y = bounds.h;
	log_line("sdl: selected display {i}, size {iv2}",
		display_index, display_size.array()
	);

	return result_e::success;
}

katha::result_e katha::platform_t::query_controllers()
{
	const int joystick_count = SDL_NumJoysticks();
	if (joystick_count < 0)
	{
		log_line("error-sdl: SDL_NumJoysticks {s}", SDL_GetError());
		return result_e::error_sdl;
	}
	if (0 == joystick_count)
	{
		log_line("no gamepads connected");
		return result_e::success;
	}

	int controller_count = 0;
	for (int i = 0; i < joystick_count; i++)
	{
		if (SDL_IsGameController(i))
		{
			controller_count++;
		}
	}

	gamepads = alloc<SDL_GameController*>(controller_count);
	for (int i = 0; i < joystick_count; i++)
	{
		if (!SDL_IsGameController(i))
		{
			continue;
		}

		SDL_GameController* controller = SDL_GameControllerOpen(i);
		if (nullptr == controller)
		{
			log_line("error-sdl: SDL_GameControllerOpen({i}) {s}",
				i, SDL_GetError()
			);
			continue;
		}

		gamepads[gamepad_count] = controller;
		gamepad_count++;

		log_line("gamepad_{i}: {s}", i, SDL_GameControllerName(controller));
	}

	log_line("joysticks: {i}, controllers: {i}", joystick_count, controller_count);
	return result_e::success;
}

katha::result_e katha::platform_t::init_with_gl()
{
	if (config.enabled(feature_e::vr) || !config.enabled(feature_e::force_opengl_es))
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	}
	else
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	}
	SDL_GL_SetSwapInterval(config.enabled(feature_e::vsync) ? 1 : 0);

	const int32_t position = SDL_WINDOWPOS_UNDEFINED_DISPLAY(
		display_index
	);

	int width = display_size.x;
	int height = display_size.y;
	int additional_flags = 0;
	if (config.enabled(feature_e::window_mode))
	{
		width = config.window_size.x;
		height = config.window_size.y;
		additional_flags |= SDL_WINDOW_RESIZABLE;
	}
	else
	{
		additional_flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
	}

	window = SDL_CreateWindow(GAME_NAME,
		position, position, width, height,
		SDL_WINDOW_OPENGL | additional_flags
	);
	if (nullptr == window)
	{
		log_line("error: SDL_CreateWindow {s}", SDL_GetError());
		return result_e::error_sdl;
	}

	result_e result = gl->init(config, window);
	if (!check_result(result, "gles::init"))
	{
		return result;
	}

#if KATHA_XR
	if (config.enabled(feature_e::vr))
	{
		// This piggy backs on existing OpenGL context
		result = xr->init(config, window);
		if (!check_result(result, "xr::init"))
		{
			xr->clear();
			config.features.unset(feature_e::vr);
			log_line("error: xr disabled");
		}
		else
		{

			result = gl->create_framebuffer(
				&(gl->framebuffers.left),
				xr->get_swapchain_size(xr_t::EYE_LEFT),
				format_e::rgba8,
				format_e::depth24_stencil8
			);
			if (!check_result(result, "gl::create_framebuffer->xr_left"))
			{
				return result_e::error_gl;
			}
	
			result = gl->create_framebuffer(
				&(gl->framebuffers.right),
				xr->get_swapchain_size(xr_t::EYE_RIGHT),
				format_e::rgba8,
				format_e::depth24_stencil8
			);
			if (!check_result(result, "gl::create_framebuffer->xr_right"))
			{
				return result_e::error_gl;
			}
		}
	}
#endif
	if (0 == config.enabled(feature_e::vr))
	{
		result = gl->create_framebuffer(
			&(gl->framebuffers.left),
			get_drawable_size(),
			format_e::rgba8,
			format_e::depth24_stencil8
		);
		if (!check_result(result, "gl::create_framebuffer::platform_main"))
		{
			return result_e::error_gl;
		}
	}

	return result_e::success;
}

void katha::platform_t::clear_gl()
{
	gl->clear();
}

katha::result_e katha::platform_t::init_with_vulkan()
{
	// Simple check to verify Vulkan exist
	if (SDL_Vulkan_LoadLibrary(nullptr) < 0)
	{
		log_line("error: {s}", SDL_GetError());
		return result_e::error;
	}
	// SDL_CreateWindow will load it again
	SDL_Vulkan_UnloadLibrary();
	
	const int32_t position = SDL_WINDOWPOS_UNDEFINED_DISPLAY(
		display_index
	);
	window = SDL_CreateWindow(GAME_NAME,
		position, position, 1280, 720,
		SDL_WINDOW_VULKAN
	);
	if (nullptr == window)
	{
		log_line("error: SDL_CreateWindow {s}", SDL_GetError());
		return result_e::error_sdl;
	}

	if (config.enabled(feature_e::vr))
	{
#if KATHA_XR
		// This will create Vulkan instance & select physical device
		result_e result = xr->init(config, window);
		if (!check_result(result, "xr::init"))
		{
			config.features.unset(feature_e::vr);
			log_line("error: xr disabled");
		}
#endif
	}
	else
	{
		result_e result = vulkan->init(config, window);
		if (!check_result(result, "vulkan::create_instance"))
		{
			return result;
		}
	}

	return result_e::success;
}

void katha::platform_t::clear_vulkan()
{
	vulkan->clear();
}

katha::uvec2 katha::platform_t::get_window_size() const
{
	int width = 0;
	int height = 0;
	SDL_GetWindowSize(window, &width, &height);
	
	uvec2 size(width, height);
	return size;
}

katha::uvec2 katha::platform_t::get_drawable_size() const
{
	int width = 0;
	int height = 0;
	switch (config.graphics_api)
	{
		case graphics_api_e::gl:
			SDL_GL_GetDrawableSize(window, &width, &height);
			break;
		
		default:
			return get_window_size();
	}

	uvec2 size(width, height);
	return size;
}

katha::result_e katha::platform_t::poll_events()
{
	SDL_Event event = {};
	while (SDL_PollEvent(&event))
	{
		if (SDL_QUIT == event.type)
		{
			return result_e::request_exit;
		}
	}

	last_input_state = current_input_state;
	current_input_state = {};

	int num_keys = 0;
	const uint8_t* key_state = SDL_GetKeyboardState(&num_keys);
	for (int i = 0; i < max(num_keys, 512); i++)
	{
		current_input_state.set_key(i, key_state[i]);
	}
	
	if (current_input_state.get_key(SDL_SCANCODE_ESCAPE))
	{
		return result_e::request_exit;
	}

	int x, y;
	SDL_GetMouseState(&x, &y);
	current_input_state.cursor = ivec2(x, y);

	current_input_state.gamepad = get_gamepad_state(0);
	for (int i = 1; i < gamepad_count; i++)
	{
		gamepad_t gp = get_gamepad_state(i);
		current_input_state.gamepad.merge(gp);
	}
	if (current_input_state.gamepad.start)
	{
		return result_e::request_exit;
	}

	return result_e::success;
}

katha::gamepad_t katha::platform_t::get_gamepad_state(const int32_t index) const
{
	if (index >= gamepad_count)
	{
		return {};
	}

	constexpr float AXIS_MAX = INT16_MAX;

	gamepad_t gamepad = {};
	SDL_GameController* controller = gamepads[index];
	if (nullptr == controller)
	{
		return {};
	}
	
	gamepad.stick_left.x = SDL_GameControllerGetAxis(controller,
		SDL_CONTROLLER_AXIS_LEFTX
	) / AXIS_MAX;
	gamepad.stick_left.y = SDL_GameControllerGetAxis(controller,
		SDL_CONTROLLER_AXIS_LEFTY
	) / AXIS_MAX;

	gamepad.stick_right.x = SDL_GameControllerGetAxis(controller,
		SDL_CONTROLLER_AXIS_RIGHTX
	) / AXIS_MAX;
	gamepad.stick_right.y = SDL_GameControllerGetAxis(controller,
		SDL_CONTROLLER_AXIS_RIGHTY
	) / AXIS_MAX;

	gamepad.trigger_left = SDL_GameControllerGetAxis(controller,
		SDL_CONTROLLER_AXIS_TRIGGERLEFT
	) / AXIS_MAX;
	gamepad.trigger_right = SDL_GameControllerGetAxis(controller,
		SDL_CONTROLLER_AXIS_TRIGGERRIGHT
	) / AXIS_MAX;

	gamepad.dpad_up = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP);
	gamepad.dpad_left = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	gamepad.dpad_down = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	gamepad.dpad_right = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

	gamepad.action_up = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y);
	gamepad.action_left = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X);
	gamepad.action_down = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A);
	gamepad.action_right = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B);

	gamepad.shoulder_left = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
	gamepad.shoulder_right = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);

	gamepad.l3 = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSTICK);
	gamepad.r3 = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSTICK);

	gamepad.select = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_BACK);
	gamepad.start = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_START);

	return gamepad;
}

katha::action_map_t katha::platform_t::get_action_map() const
{
	action_map_t map = {};
	const gamepad_t& gp = current_input_state.gamepad;

	constexpr float stick_dead_zone = 0.05f;

	// steering
	map.steering_angle = gp.stick_left.x;
	if (gp.dpad_left || get_key(SDL_SCANCODE_A) || get_key(SDL_SCANCODE_LEFT))
	{
		map.steering_angle -= 1.0f;
	}
	if (gp.dpad_right || get_key(SDL_SCANCODE_D) || get_key(SDL_SCANCODE_RIGHT))
	{
		map.steering_angle += 1.0f;
	}

	// throttle
	map.throttle = clamp(gp.trigger_right - stick_dead_zone, 0.0f, 1.0f);
	if (gp.stick_right.y < 0)
	{
		map.throttle += clamp(-gp.stick_right.y - 0.05f, 0.0f, 1.0f);
	}
	if (gp.action_down || get_key(SDL_SCANCODE_W) || get_key(SDL_SCANCODE_UP))
	{
		map.throttle += 1.0f;
	}

	// brake
	map.brake = gp.trigger_left;
	if (gp.stick_right.y > 0)
	{
		map.brake += clamp(gp.stick_right.y - stick_dead_zone, 0.0f, 1.0f);
	}
	if (gp.action_right || get_key(SDL_SCANCODE_S) || get_key(SDL_SCANCODE_DOWN))
	{
		map.brake += 1.0f;
	}

	map.steering_angle = clamp(map.steering_angle, -1.0f, 1.0f);
	map.throttle = clamp(map.throttle, 0.0f, 1.0f);
	map.brake = clamp(map.brake, 0.0f, 1.0f);

	return map;
}