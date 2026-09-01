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
	struct platform_t
	{
		static platform_t* get();

		char* base_path = nullptr;
		string_t asset_root = {};

		SDL_Window* window = nullptr;
		gl_t gl = {};

		result_e init(int argc, char** args);
		void clear();

		result_e init_graphics();

		[[nodiscard]]
		result_t<file_t> open_file_read(const char* file);
	};
}

#endif