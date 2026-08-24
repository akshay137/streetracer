#include "gl.hpp"
#include "../../katha/core/config.hpp"
#include "../../katha/physics/vertex.hpp"

#include <SDL2/SDL_video.h>
#include <glad/glad.h>

katha::gl_t gl_global_instance = {};
katha::gl_t * const katha::gl = &gl_global_instance;

constexpr const char* vertex_shader_mesh = ""
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec2 tex_coords;\n"
"layout (location = 0) uniform mat4 mvp;\n"
"out vec2 uv;\n"
"void main()\n"
"{\n"
	"gl_Position = mvp * vec4(position, 1);\n"
	"uv = tex_coords;"
"}\n"
;

constexpr const char* fragment_shader_mesh = ""
"in MEDIUMP vec2 uv;\n"
"layout (location = 0) out MEDIUMP vec4 color;\n"
"layout (binding = 0, location = 1) uniform sampler2D diffuse;\n"
"void main()\n"
"{\n"
	"color = texture(diffuse, uv);\n"
"}\n"
;

extern "C" void gl_debug_callback(GLenum source,
	GLenum type, GLuint id,
	GLenum severity, GLsizei length,
	const GLchar* message,
	const void* user_param
)
{
	const char* severity_s = "unknown";
	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH: severity_s = "high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM: severity_s = "medium"; break;
		case GL_DEBUG_SEVERITY_LOW: severity_s = "low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: severity_s = "notification"; break;
	}

	const char* source_s = "unknown";
	switch (source)
	{
		case GL_DEBUG_SOURCE_API: source_s = "api"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: source_s = "window system"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: source_s = "shader compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY: source_s = "third party"; break;
		case GL_DEBUG_SOURCE_APPLICATION: source_s = "application"; break;
		case GL_DEBUG_SOURCE_OTHER: source_s = "other"; break;
	}

	const char* type_s = "unknown";
	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR: type_s = "error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_s = "deprecated behavior"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: type_s = "undefined behavior"; break;
		case GL_DEBUG_TYPE_PORTABILITY: type_s = "portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE: type_s = "performance"; break;
		case GL_DEBUG_TYPE_MARKER: type_s = "marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP: type_s = "push group"; break;
		case GL_DEBUG_TYPE_POP_GROUP: type_s = "pop group"; break;
		case GL_DEBUG_TYPE_OTHER: type_s = "other"; break;
	}

	katha::log_line(
		"gl_debug_{u}: source {s}, type {s}, severity {s}\n"
		"message: {s}",
		id, source_s, type_s, severity_s, message
	);
}

katha::result_e katha::gl_t::init(const config_t& config, SDL_Window* window)
{
	log_line("gl::init()");
	context = SDL_GL_CreateContext(window);
	if (nullptr == context)
	{
		log_line("error-sdl: SDL_GL_CreateContext {s}", SDL_GetError());
		return result_e::error_sdl;
	}

	this->window = window;
	log_line("OpenGL context: {p}", context);

	// We only use what is available in both OpenGL 4.5 & OpenGL ES 3.1
	// glad names it GLES2 but internally loads functions for ES 3.x
	if (0 == gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		log_line("error: failed to load gl procs");
		return result_e::error_gl;
	}

	int32_t version_major = 0;
	int32_t version_minor = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &version_major);
	glGetIntegerv(GL_MINOR_VERSION, &version_minor);
	version.major = static_cast<uint32_t>(version_major);
	version.minor = static_cast<uint32_t>(version_minor);

	const char* version_string = (const char*)glGetString(GL_VERSION);
	if (string_t::cstring_starts_with(version_string, "OpenGL ES"))
	{
		es_context = 1;
		if (!version.is_at_least(version_t(3, 1, 0)))
		{
			log_line("error-gl: failed to create OpenGL ES 3.1 context");
			return result_e::error_gl;
		}
	}
	else
	{
		es_context = 0;
		if (!version.is_at_least(version_t(4, 5, 0)))
		{
			log_line("error-gl: failed to create OpenGL 4.5 context");
			return result_e::error_gl;
		}
	}

	check_extensions();

	if (ext_debug && config.debug_graphics_api)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(gl_debug_callback, nullptr);
		glEnable(0xffff);
	}

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);

	// don't need to change this, a sensible global default
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	log_line("OpenGL Version: {s}", glGetString(GL_VERSION));
	log_line("Vendor: {s}", glGetString(GL_VENDOR));
	log_line("Renderer: {s}", glGetString(GL_RENDERER));
	log_line("GL_MAX_TEXTURE_SIZE: {i}", max_texture_size);

	result_e result = create_pso(
		&pso_mesh,
		vertex_layout_e::mesh,
		vertex_shader_mesh,
		fragment_shader_mesh,
		blend_mode_e::none,
		depth_mode_e::less
	);
	if (!check_result(result, "gl::create_pso_mesh"))
	{
		return result;
	}

	return result_e::success;
}

void katha::gl_t::clear()
{
	log_line("gl::clear()");

	delete_framebuffer(&(framebuffers.left));
	delete_framebuffer(&(framebuffers.right));
	delete_pso(&pso_mesh);

	SDL_GL_DeleteContext(context);
	context = nullptr;
}

void katha::gl_t::check_extensions()
{
	GLint count = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &count);

	if (0 == count)
	{
		return;
	}

	for (GLint i = 0; i < count; i++)
	{
		const string_t extension = (const char*)glGetStringi(GL_EXTENSIONS, i);

		if (extension.equals("GL_KHR_debug"))
		{
			ext_debug = 1;
		}
	}
}

void katha::gl_t::clear_screen(const vec4& color)
{
	glClearBufferfv(GL_COLOR, 0, &(color.x));
	glClearBufferfi(GL_DEPTH_STENCIL, 0, 1, 0);
}

const char* katha::gl_t::get_shader_version_string() const
{
	if (es_context)
	{
		return "#version 310 es\n"
			"#define HIGHP highp\n"
			"#define MEDIUMP mediump\n"
			"#define LOWP lowp\n";
	}

	return "#version 450\n"
		"#define HIGHP\n"
		"#define MEDIUMP\n"
		"#define LOWP\n";
}

void katha::gl_t::bind_framebuffer(const framebuffer_t& framebuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(framebuffer.id));
	glViewport(0, 0, framebuffer.size.x, framebuffer.size.y);
}

void katha::gl_t::blit_to_screen(const framebuffer_t& framebuffer,
	const ivec4& screen_rect,
	const bool filter_linear
)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, static_cast<GLuint>(framebuffer.id));
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBlitFramebuffer(
		0, 0, framebuffer.size.x, framebuffer.size.y,
		screen_rect.x, screen_rect.y, screen_rect.z, screen_rect.w,
		GL_COLOR_BUFFER_BIT, filter_linear ? GL_LINEAR : GL_NEAREST
	);
}

void katha::gl_t::blit_to_framebuffer(
	const framebuffer_t& source,
	const framebuffer_t& target,
	const bool filter_linear
)
{
	if (!(source.id && target.id))
	{
		log_line("warning-gl: blit_to_framebuffer(source={u64:x}, target={u64:x})",
			source.id,
			target.id
		);
		return;
	}

	glBindFramebuffer(GL_READ_FRAMEBUFFER, static_cast<GLuint>(source.id));
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, static_cast<GLuint>(target.id));

	glBlitFramebuffer(
		0, 0, static_cast<GLint>(source.size.x), static_cast<GLint>(source.size.y),
		0, 0, static_cast<GLint>(target.size.x), static_cast<GLint>(target.size.y),
		GL_COLOR_BUFFER_BIT, filter_linear ? GL_LINEAR : GL_NEAREST
	);
}

void katha::gl_t::bind_texture(const texture_t& texture, const uint32_t slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(texture.id));
}

bool katha::gl_t::set_blend_mode(const blend_mode_e mode)
{
	if (blend_mode_e::none == mode)
	{
		glDisable(GL_BLEND);
		return true;
	}

	if (blend_mode_e::one_minus_src_alpha == mode)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		return true;
	}

	log_line("warn-gl: unknown blend mode {u}", static_cast<uint32_t>(mode));
	return false;
}

bool katha::gl_t::set_depth_mode(const depth_mode_e mode)
{
	if (depth_mode_e::none == mode)
	{
		glDisable(GL_DEPTH_TEST);
		return true;
	}

	if (depth_mode_e::less == mode)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		return true;
	}

	if (depth_mode_e::greater == mode)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_GREATER);
		return true;
	}

	log_line("warn-gl: unknwon depth mode {u}", static_cast<uint32_t>(mode));
	return false;
}

void katha::gl_t::use_pso(const pso_t& pso)
{
	if (0 == pso.id)
	{
		log_line("warning-gl: use_pso(0)");
		return;
	}

	glUseProgram(static_cast<GLuint>(pso.id));
	set_blend_mode(pso.blend_mode);
	set_depth_mode(pso.depth_mode);

	glBindVertexArray(static_cast<GLuint>(pso.layout));
}

void katha::gl_t::set_uniform_texture_unit(const int32_t location, const uint32_t unit)
{
	glUniform1i(location, unit);
}

void katha::gl_t::set_uniform_vec3(const int32_t location, const vec3& v)
{
	glUniform3f(location, v.x, v.y, v.z);
}

void katha::gl_t::set_uniform_quat(const int32_t location, const quat_t& q)
{
	glUniform4f(location, q.x, q.y, q.z, q.w);
}

void katha::gl_t::set_uniform_mat4(const int32_t location, const float* m)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, m);
}

void katha::gl_t::set_vertex_buffer(
	const buffer_t& buffer,
	const uint32_t index,
	const uint32_t offset,
	const uint32_t stride
)
{
	glBindVertexBuffer(index,
		static_cast<GLuint>(buffer.id),
		static_cast<GLintptr>(offset),
		static_cast<GLsizei>(stride)
	);
}

uint32_t katha::gl_t::create_vao_vertex_t(const bool bind)
{
	uint32_t vao = 0;
	glGenVertexArrays(1, &vao);
	if (0 == vao)
	{
		log_line("error-gl: failed to create vao for vertex_t");
		return 0;
	}

	glBindVertexArray(vao);

	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(vertex_t, position));
	glVertexAttribBinding(0, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribFormat(1, 2, GL_UNSIGNED_SHORT, GL_TRUE, offsetof(vertex_t, uv));
	glVertexAttribBinding(1, 0);
	glEnableVertexAttribArray(1);

	if (!bind)
	{
		glBindVertexArray(0);
	}

	log_line("gl: vertex_array_object<vertex_t> {u}", vao);
	return vao;
}

void katha::gl_t::draw_arrays(uint32_t vertices)
{
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices));
}

void katha::gl_t::set_viewport(const ivec2 size)
{
	glViewport(0, 0,
		static_cast<GLsizei>(size.x),
		static_cast<GLsizei>(size.y)
	);
}

katha::gl_t::format_t katha::gl_t::format_to_gl_format(const format_e format)
{
	switch (format)
	{
		case format_e::rgba8:
			return {
				.internal_format = GL_RGBA8,
				.channel = GL_RGBA,
				.data_type = GL_UNSIGNED_BYTE
			};

		case format_e::rgb8:
			return {
				.internal_format = GL_RGB8,
				.channel = GL_RGB,
				.data_type = GL_UNSIGNED_BYTE
			};
		
		case format_e::srgba8:
			return {
				.internal_format = GL_SRGB8_ALPHA8,
				.channel = GL_RGBA,
				.data_type = GL_UNSIGNED_BYTE
			};
			
		case format_e::srgb8:
			return {
				.internal_format = GL_SRGB8,
				.channel = GL_RGB,
				.data_type = GL_UNSIGNED_BYTE
			};
		
		case format_e::depth24_stencil8:
			return {
				.internal_format = GL_DEPTH24_STENCIL8,
				.channel = GL_DEPTH_STENCIL,
				.data_type = GL_UNSIGNED_INT_24_8
			};
	}

	log_line("error: format_to_gl_format({s}) -> unsupported format",
		format_to_cstring(format)
	);
	return {};
}