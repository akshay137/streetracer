#pragma once
#ifndef KATHA_PLATFORM_DESKTOP_H__
#define KATHA_PLATFORM_DESKTOP_H__ 1

#include "../katha/core.hpp"
#include "../katha/core/config.hpp"
#include "input.hpp"
#include "gamepad.hpp"

#include "../game/action_map.hpp"

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_gamecontroller.h>

namespace katha
{
	struct platform_t
	{
		const char* base_path = nullptr;
		const char* user_path = nullptr;
		int32_t display_index = 0;
		ivec2 display_size = {};

		SDL_Window* window = nullptr;

		config_t config = {};

		int gamepad_count = 0;
		SDL_GameController** gamepads = nullptr;

		input_t last_input_state = {};
		input_t current_input_state = {};

		result_e init(int argc, char** args);
		void clear();

		void parse_command_line(int argc, char** args);

		result_e query_displays();
		result_e query_controllers();

		// initialized sdl2, openxr and opengles
		result_e init_with_gl();
		void clear_gl();

		result_e init_with_vulkan();
		void clear_vulkan();

		ivec2 get_window_size() const;
		ivec2 get_drawable_size() const;

		result_e poll_events();
		gamepad_t get_gamepad_state(const int32_t index) const;

		action_map_t get_action_map() const;

		ivec2 get_cursor_position() const
		{
			return current_input_state.cursor;
		}

		bool get_key(const int key) const
		{
			return current_input_state.get_key(key);
		}

		bool is_key_pressed(const int key) const
		{
			const bool last = last_input_state.get_key(key);
			const bool current = current_input_state.get_key(key);
			return !last && current;
		}

		bool is_key_released(const int key) const
		{
			const bool last = last_input_state.get_key(key);
			const bool current = current_input_state.get_key(key);
			return last && !current;
		}
	};
}

#endif