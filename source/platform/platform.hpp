#pragma once
#ifndef KATHA_PLATFORM_DESKTOP_H__
#define KATHA_PLATFORM_DESKTOP_H__ 1

#include "../config.hpp"
#include "../type/action_map.hpp"
#include "../type/file.hpp"
#include "../type/gamepad.hpp"
#include "../type/result.hpp"
#include "../type/string.hpp"
#include "../graphics/gl.hpp"
#include "input.hpp"

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_gamecontroller.h>

namespace katha
{
	typedef void (*pfn_on_force_exit)();

	struct platform_t
	{
		static platform_t* get();

		pfn_on_force_exit on_force_exit = nullptr;
		char* base_path = nullptr;
		string_t asset_root = {};

		SDL_Window* window = nullptr;
		gl_t gl = {};

		result_e init(int argc, char** args);
		void clear();

		result_e init_graphics();

		[[noreturn]]
		void force_exit(const source_t& source = source_t::current());
		void set_force_exit_callback(pfn_on_force_exit callback);

		[[nodiscard]]
		result_t<file_t> open_file_read(const char* file);
	};
}

#endif