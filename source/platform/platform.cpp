#include "platform.hpp"
#include "../type/command_line.hpp"
#include "../utility.hpp"

#include <SDL2/SDL.h>

katha::Platform* katha::Platform::Get()
{
	static Platform platform = {};
	return &platform;
}

katha::Result katha::Platform::init(int argc, char** args)
{
	CommandLine::Parse(argc, args);
	CommandLine::Log();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
	{
		LogLine("error-sdl: SDL_Init {s}", SDL_GetError());
		return Result::ERROR_SDL;
	}

	base_path = SDL_GetBasePath();
	if (nullptr == base_path)
	{
		LogLine("error-sdl: SDL_GetBasePath {s}", SDL_GetError());
		return Result::ERROR_SDL;
	}
	LogLine("base_path: {s}", base_path);
	asset_root = CommandLine::GetAssetRoot(base_path);

	Result result = initGraphics();
	if (!CheckResult(result, "platform::init_graphics"))
	{
		return result;
	}

	return Result::SUCCESS;
}

void katha::Platform::clear()
{
	gles.clear();
	
	if (window)
	{
		SDL_DestroyWindow(window);
		window = nullptr;
	}

	SDL_free(base_path);
	SDL_Quit();
}

katha::Result katha::Platform::initGraphics()
{
	// TODO
	if (true)
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

	int display = CommandLine::GetDisplayIndex(0);
	int pos = SDL_WINDOWPOS_UNDEFINED_DISPLAY(display);
	int width = 1280;
	int height = 720;

	window = SDL_CreateWindow("StreetRacer", pos, pos, width, height, SDL_WINDOW_OPENGL);
	if (nullptr == window)
	{
		LogLine("error-sdl: SDL_CreateWindow {s}", SDL_GetError());
		return Result::ERROR_SDL;
	}
	LogLine("window: {p}", window);

	Result result = gles.init();
	if (!CheckResult(result, "gl::init"))
	{
		return result;
	}

	return Result::SUCCESS;
}

void katha::Platform::forceExit(const source_t& source)
{
	LogLine("force_exit called from {src}", &source);
	
	if (on_force_exit)
	{
		on_force_exit();
	}

	clear();
	exit(0);
}

void katha::Platform::setForceExitCallback(PFN_OnForceExit callback)
{
	on_force_exit = callback;
}

katha::File katha::Platform::openFileRead(const char* file)
{
	char full_path[1024] = {};
	if (String::JoinPath(asset_root, file, full_path, 1024))
	{
		File _file = File::OpenRead(full_path);
		return _file;
	}

	String path = String::JoinPath(asset_root, file);
	File _file = File::OpenRead(path.buffer);
	path.clear();
	return _file;
}