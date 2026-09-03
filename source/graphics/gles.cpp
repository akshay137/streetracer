#include "gles.hpp"
#include "../type/command_line.hpp"
#include "../type/string.hpp"
#include "../platform/platform.hpp"
#include "../utility.hpp"

#include <glad/glad.h>
#include <SDL2/SDL_video.h>

APIENTRY void GLESDebugCallback(
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

	katha::LogLine("debug-gles: [ id:{u} | {s} | {s} | {s} ] {s}",
		id, source_str, type_str, severity_str, message
	);
}

katha::Result katha::GLES::init()
{
	window = Platform::Get()->window;
	context = SDL_GL_CreateContext(window);
	if (nullptr == context)
	{
		LogLine("error-sdl: SDL_GL_CreateContext {s}", SDL_GetError());
		return Result::ERROR_SDL;
	}
	LogLine("gl_context: {p}", context);

	if (0 == gladLoadGLES2Loader(SDL_GL_GetProcAddress))
	{
		LogLine("error-glad: failed to load procs");
		return Result::ERROR_GLES;
	}
	if (!checkVersion())
	{
		return Result::ERROR_VALUE_UNEXPECTED;
	}

	queryExtensions();
	queryLimits();

	if (
		CommandLine::Has(CommandLine::Command::DEBUG_GRAPHICS)
		&& extensions.hasEnum(Extension::gl_khr_debug)
	)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLESDebugCallback, nullptr);
		glEnable(0xffff);
		checkError();
	}

	LogLine("{s}", glGetString(GL_VERSION));

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	if (!createResources())
	{
		return Result::ERROR_GLES;
	}

	return Result::SUCCESS;
}

void katha::GLES::clear()
{
	if (nullptr == context)
	{
		return;
	}

	clearResources();

	SDL_GL_DeleteContext(context);
	context = nullptr;
}

bool katha::GLES::checkError(const source_t& source)
{
	GLenum error = glGetError();
	bool has_error = false;
	while (GL_NO_ERROR != error)
	{
		switch (error)
		{
			case GL_INVALID_ENUM:
				has_error = true;
				LogLine("error-gles: GL_INVALID_ENUM at {src}", &source);
				break;
			
			case GL_INVALID_VALUE:
				has_error = true;
				LogLine("error-gles: GL_INVALID_VALUE at {src}", &source);
				break;

			case GL_INVALID_OPERATION:
				has_error = true;
				LogLine("error-gles: GL_INVALID_OPERATION at {src}", &source);
				break;

			case GL_INVALID_FRAMEBUFFER_OPERATION:
				has_error = true;
				LogLine("error-gles: GL_INVALID_FRAMEBUFFER_OPERATION at {src}", &source);
				break;
			
			case GL_OUT_OF_MEMORY:
				has_error = true;
				LogLine("error-gles: GL_OUT_OF_MEMORY at {src}", &source);
				break;
		}

		error = glGetError();
	}

	return has_error;
}

bool katha::GLES::checkVersion()
{
	const char* version_string = (const char*)glGetString(GL_VERSION);
	is_es_context = String::CStringStartsWith(version_string, "OpenGL ES");

	GLint major = 0;
	GLint minor = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	version = Version(major, minor, 0);

	if (is_es_context)
	{
		constexpr Version min_es_version = Version(3, 1, 0);
		if (version < min_es_version)
		{
			LogLine("error-gles: version mismatch: required {version}, has {version}",
				&min_es_version, &version
			);
			return false;
		}
		return true;
	}

	constexpr Version min_core_version = Version(4, 5, 0);
	if (version < min_core_version)
	{
		LogLine("error-gles: version mismatch: required {version}, has {version}",
			&min_core_version, &version
		);
		return false;
	}
	return true;
}

void katha::GLES::queryExtensions()
{
	GLint count = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &count);
	if (0 == count)
	{
		LogLine("warn-gles: no extensions found");
		return;
	}

	for (GLint i = 0; i < count; i++)
	{
		const String ext = (const char*)glGetStringi(GL_EXTENSIONS, i);

		if (ext.equals("GL_KHR_debug"))
		{
			extensions.setEnum(Extension::gl_khr_debug);
		}
	}
}

void katha::GLES::queryLimits()
{}

katha::GLESFormat katha::GLES::FormatToGLESFormat(const Format format)
{
	switch (format)
	{
		case Format::NONE: return {};

		case Format::GREYSCALE8:
			return {
				.internal = GL_R8,
				.channel = GL_RED,
				.data_type = GL_UNSIGNED_BYTE,
				.swizzle = ivec4(GL_RED, GL_RED, GL_RED, GL_ONE)
			};

		case Format::RGBA8:
			return {
				.internal = GL_RGBA8,
				.channel = GL_RGBA,
				.data_type = GL_UNSIGNED_BYTE,
				.swizzle = ivec4(GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA)
			};
		
		case Format::RGB8:
			return {
				.internal = GL_RGB8,
				.channel = GL_RGB,
				.data_type = GL_UNSIGNED_BYTE,
				.swizzle = ivec4(GL_RED, GL_GREEN, GL_BLUE, GL_ONE)
			};

		case Format::SRGBA8:
			return {
				.internal = GL_SRGB8_ALPHA8,
				.channel = GL_RGBA,
				.data_type = GL_UNSIGNED_BYTE,
				.swizzle = ivec4(GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA)
			};

		case Format::DEPTH24_STENCIL8:
			return {
				.internal = GL_DEPTH24_STENCIL8,
				.channel = GL_DEPTH_STENCIL,
				.data_type = GL_UNSIGNED_INT_24_8,
				.swizzle = {}
			};
	}

	LogLine("gles: unsupported format {s}|{i}",
		FormatToCString(format),
		static_cast<int>(format)
	);
	return {};
}