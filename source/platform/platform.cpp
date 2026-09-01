#include "platform.hpp"
#include "../type/command_line.hpp"
#include "../utility.hpp"

#include <SDL2/SDL.h>

katha::platform_t* katha::platform_t::get()
{
	static platform_t platform = {};
	return &platform;
}

katha::result_e katha::platform_t::init(int argc, char** args)
{
	command_line::parse(argc, args);
	command_line::log();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
	{
		log_line("error-sdl: SDL_Init {s}", SDL_GetError());
		return result_e::error_sdl;
	}

	base_path = SDL_GetBasePath();
	if (nullptr == base_path)
	{
		log_line("error-sdl: SDL_GetBasePath {s}", SDL_GetError());
		return result_e::error_sdl;
	}
	log_line("base_path: {s}", base_path);
	asset_root = command_line::get_asset_root(base_path);

	result_e result = init_graphics();
	if (!check_result(result, "platform::init_graphics"))
	{
		return result;
	}

	return result_e::success;
}

void katha::platform_t::clear()
{
	gl.clear();
	
	if (window)
	{
		SDL_DestroyWindow(window);
		window = nullptr;
	}

	SDL_free(base_path);
	SDL_Quit();
}

katha::result_e katha::platform_t::init_graphics()
{
	if (command_line::has(command_line::command::force_opengl_es))
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	}
	else
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	}

	int display = command_line::get_display_index(0);
	int pos = SDL_WINDOWPOS_UNDEFINED_DISPLAY(display);
	int width = 1280;
	int height = 720;

	window = SDL_CreateWindow("StreetRacer", pos, pos, width, height, SDL_WINDOW_OPENGL);
	if (nullptr == window)
	{
		log_line("error-sdl: SDL_CreateWindow {s}", SDL_GetError());
		return result_e::error_sdl;
	}
	log_line("window: {p}", window);

	result_e result = gl.init(this);
	if (!check_result(result, "gl::init"))
	{
		return result;
	}

	return result_e::success;
}

void katha::platform_t::force_exit(const source_t& source)
{
	log_line("force_exit called from {src}", &source);
	
	if (on_force_exit)
	{
		on_force_exit();
	}

	clear();
	exit(0);
}

void katha::platform_t::set_force_exit_callback(pfn_on_force_exit callback)
{
	on_force_exit = callback;
}

katha::result_t<katha::file_t> katha::platform_t::open_file_read(const char* file)
{
	string_t path = string_t::join_path(asset_root, file);
	file_t f = file_t::open_read(path.buffer);
	path.clear();
	return f;
}