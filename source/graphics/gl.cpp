#include "gl.hpp"
#include "../type/command_line.hpp"
#include "../type/string.hpp"
#include "../platform/platform.hpp"
#include "../utility.hpp"

#include <glad/glad.h>
#include <SDL2/SDL_video.h>

APIENTRY void gl_debug_callback(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *user_param
)
{
	const char* source_str = "source:unknown";
	const char* type_str = "type:unknown";
	const char* severity_str = "severity:unknown";

	switch (source)
	{
		case GL_DEBUG_SOURCE_API: source_str = "source:api"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: source_str = "source:window_system"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: source_str = "source:shader_compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY: source_str = "source:third_party"; break;
		case GL_DEBUG_SOURCE_APPLICATION: source_str = "source:application"; break;
		case GL_DEBUG_SOURCE_OTHER: source_str = "source:other"; break;
	}

	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR: type_str = "type:error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_str = "type:deprecated_behavior"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: type_str = "type:undefined_behavior"; break;
		case GL_DEBUG_TYPE_PORTABILITY: type_str = "type:portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE: type_str = "type:performance"; break;
		case GL_DEBUG_TYPE_MARKER: type_str = "type:marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP: type_str = "type:push_group"; break;
		case GL_DEBUG_TYPE_POP_GROUP: type_str = "type:pop_group"; break;
		case GL_DEBUG_TYPE_OTHER: type_str = "type:other"; break;
	}

	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH: severity_str = "severity:high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM: severity_str = "severity:medium"; break;
		case GL_DEBUG_SEVERITY_LOW: severity_str = "severity:low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: severity_str = "severity:notification"; break;
	}

	katha::log_line("debug-gl: [ id:{u} | {s} | {s} | {s} ] {s}",
		id, source_str, type_str, severity_str, message
	);
}

katha::result_e katha::gl_t::init(platform_t* platform)
{
	context = SDL_GL_CreateContext(platform->window);
	if (nullptr == context)
	{
		log_line("error-sdl: SDL_GL_CreateContext {s}", SDL_GetError());
		return result_e::error_sdl;
	}
	log_line("gl_context: {p}", context);
	window = platform->window;

	if (0 == gladLoadGLES2Loader(SDL_GL_GetProcAddress))
	{
		log_line("error-glad: failed to load procs");
		return result_e::error_gl;
	}
	if (!check_version())
	{
		return result_e::error_value_unexpected;
	}

	query_extensions();
	query_limits();

	if (
		command_line::has(command_line::command::debug_graphics)
		&& extensions.has_enum(extension_e::gl_khr_debug)
	)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(gl_debug_callback, nullptr);
		glEnable(0xffff);
		check_error();
	}

	log_line("{s}", glGetString(GL_VERSION));

	if (!create_resources())
	{
		return result_e::error_gl;
	}

	return result_e::success;
}

void katha::gl_t::clear()
{
	if (nullptr == context)
	{
		return;
	}

	clear_resources();

	SDL_GL_DeleteContext(context);
	context = nullptr;
}

bool katha::gl_t::check_error(const source_t& source)
{
	GLenum error = glGetError();
	bool has_error = false;
	while (GL_NO_ERROR != error)
	{
		switch (error)
		{
			case GL_INVALID_ENUM:
				has_error = true;
				log_line("error-gl: GL_INVALID_ENUM at {src}", &source);
				break;
			
			case GL_INVALID_VALUE:
				has_error = true;
				log_line("error-gl: GL_INVALID_VALUE at {src}", &source);
				break;

			case GL_INVALID_OPERATION:
				has_error = true;
				log_line("error-gl: GL_INVALID_OPERATION at {src}", &source);
				break;

			case GL_INVALID_FRAMEBUFFER_OPERATION:
				has_error = true;
				log_line("error-gl: GL_INVALID_FRAMEBUFFER_OPERATION at {src}", &source);
				break;
			
			case GL_OUT_OF_MEMORY:
				has_error = true;
				log_line("error-gl: GL_OUT_OF_MEMORY at {src}", &source);
				break;
		}

		error = glGetError();
	}

	return has_error;
}

bool katha::gl_t::check_version()
{
	const char* version_string = (const char*)glGetString(GL_VERSION);
	is_es_context = string_t::cstring_starts_with(version_string, "OpenGL ES");

	GLint major = 0;
	GLint minor = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	version = version_t(major, minor, 0);

	if (is_es_context)
	{
		constexpr version_t min_es_version = version_t(3, 1, 0);
		if (version < min_es_version)
		{
			log_line("error-gl: version mismatch: required {version}, has {version}",
				&min_es_version, &version
			);
			return false;
		}
		return true;
	}

	constexpr version_t min_core_version = version_t(4, 5, 0);
	if (version < min_core_version)
	{
		log_line("error-gl: version mismatch: required {version}, has {version}",
			&min_core_version, &version
		);
		return false;
	}
	return true;
}

void katha::gl_t::query_extensions()
{
	GLint count = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &count);
	if (0 == count)
	{
		log_line("warn-gl: no extensions found");
		return;
	}

	for (GLint i = 0; i < count; i++)
	{
		const string_t ext = (const char*)glGetStringi(GL_EXTENSIONS, i);

		if (ext.equals("GL_KHR_debug"))
		{
			extensions.set_enum(extension_e::gl_khr_debug);
		}
	}
}

void katha::gl_t::query_limits()
{}