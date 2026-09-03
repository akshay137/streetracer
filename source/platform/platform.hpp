#pragma once
#ifndef KATHA_PLATFORM_DESKTOP_H__
#define KATHA_PLATFORM_DESKTOP_H__ 1

#include "../config.hpp"
#include "../enum/result.hpp"
#include "../type/action_map.hpp"
#include "../type/file.hpp"
#include "../type/gamepad.hpp"
#include "../type/string.hpp"
#include "../graphics/gles.hpp"
#include "input.hpp"

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_gamecontroller.h>

namespace katha
{
	typedef void (*PFN_OnForceExit)();

	struct Platform
	{
		static Platform* Get();

		PFN_OnForceExit on_force_exit = nullptr;
		char* base_path = nullptr;
		String asset_root = {};

		SDL_Window* window = nullptr;
		GLES gles = {};

		Result init(int argc, char** args);
		void clear();

		Result initGraphics();

		[[noreturn]]
		void forceExit(const source_t& source = source_t::current());
		void setForceExitCallback(PFN_OnForceExit callback);

		[[nodiscard]]
		File openFileRead(const char* file);
	};
}

#endif