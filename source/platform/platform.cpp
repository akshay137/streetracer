#include "platform.hpp"
#include "../type/command_line.hpp"
#include "../utility.hpp"

#include <SDL2/SDL.h>

katha::result_e katha::platform_t::init(int argc, char** args)
{
	command_line::parse(argc, args);
	command_line::log();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
	{
		log_line("error-sdl: SDL_Init {s}", SDL_GetError());
		return result_e::error_sdl;
	}

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