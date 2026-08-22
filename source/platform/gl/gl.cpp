#include "gl.hpp"
#include "../../katha/core/config.hpp"
#include "../../katha/physics/vertex.hpp"
#include "../../katha/math/vector2.hpp"

#include <SDL2/SDL_video.h>
#include <glad/glad.h>

katha::gl_t gl_global_instance = {};
katha::gl_t * const katha::gl = &gl_global_instance;

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

	log_line("OpenGL context: {p}", context);

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

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	log_line("OpenGL Version: {s}", glGetString(GL_VERSION));
	log_line("Vendor: {s}", glGetString(GL_VENDOR));
	log_line("Renderer: {s}", glGetString(GL_RENDERER));
	log_line("GL_MAX_TEXTURE_SIZE: {i}", max_texture_size);

	return result_e::success;
}

void katha::gl_t::clear()
{
	log_line("gl::clear()");

	delete_framebuffer(&left);
	delete_framebuffer(&right);

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

katha::result_e katha::gl_t::create_framebuffer_from_texture(
	framebuffer_t* out_framebuffer,
	const texture_t& color_0,
	const texture_t& depth_stencil
)
{
	if (nullptr == out_framebuffer)
	{
		return result_e::error_value_null;
	}

	uint32_t framebuffer = 0;
	glGenFramebuffers(1, &framebuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		static_cast<GLuint>(color_0.id), 0
	);

	if (depth_stencil.id)
	{
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
			GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
			static_cast<GLuint>(depth_stencil.id), 0
		);
	}

	GLenum gl_result = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (GL_FRAMEBUFFER_COMPLETE != gl_result)
	{
		glDeleteFramebuffers(1, &framebuffer);
		log_line("error-gl: framebuffer is incomplete {x32}", gl_result);
		return result_e::error_gl;
	}

	out_framebuffer->id = static_cast<uint64_t>(framebuffer);
	out_framebuffer->color_0 = color_0;
	out_framebuffer->depth_stencil = depth_stencil;

	out_framebuffer->log(true);
	return result_e::success;
}

katha::result_e katha::gl_t::create_framebuffer(
	framebuffer_t* out_framebuffer,
	const uvec2 size,
	const format_e color_format,
	const format_e depth_stencil_format
)
{
	log_line("gl: create_framebuffer("
		"size={iv2}, color_texture_format={s}, depth_texture_format={s}"
		")",
		size.array(),
		format_to_cstring(color_format),
		format_to_cstring(depth_stencil_format)
	);

	framebuffer_t framebuffer = {};
	framebuffer.size = size;
	
	result_e result = create_texture(
		&(framebuffer.color_0), size, color_format
	);
	if (!check_result(result, "gl::create_framebuffer::create_texture::color_0"))
	{
		return result_e::error_gl;
	}

	if (format_e::none != depth_stencil_format)
	{
		result = create_texture(
			&(framebuffer.depth_stencil), size, depth_stencil_format
		);
		if (!check_result(result, "gl::create_framebuffer::create_texture::depth_stencil"))
		{
			delete_texture(&(framebuffer.color_0));
			return result_e::error_gl;
		}
	}

	result = create_framebuffer_from_texture(&framebuffer,
		framebuffer.color_0,
		framebuffer.depth_stencil
	);
	if (!check_result(result, "create_framebuffer_from_texture"))
	{
		return result;
	}

	*out_framebuffer = framebuffer;
	return result_e::success;
}

void katha::gl_t::delete_framebuffer(framebuffer_t* framebuffer)
{
	if ((nullptr == framebuffer) || (0 == framebuffer->id))
	{
		return;
	}

	GLuint fbo = static_cast<GLuint>(framebuffer->id);
	log_line("gl: deleting framebuffer {u:x}:{iv2}",
		fbo,
		framebuffer->size.array()
	);

	glDeleteFramebuffers(1, &fbo);
	delete_texture(&(framebuffer->color_0));
	delete_texture(&(framebuffer->depth_stencil));
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
	glBindFramebuffer(GL_READ_FRAMEBUFFER, static_cast<GLuint>(source.id));
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, static_cast<GLuint>(target.id));

	glBlitFramebuffer(
		0, 0, static_cast<GLint>(source.size.x), static_cast<GLint>(source.size.y),
		0, 0, static_cast<GLint>(target.size.x), static_cast<GLint>(target.size.y),
		GL_COLOR_BUFFER_BIT, filter_linear ? GL_LINEAR : GL_NEAREST
	);
}

katha::result_e katha::gl_t::create_texture(
	texture_t* out_texture,
	const uvec2 size,
	const format_e format,
	const void* data
)
{
	if (nullptr == out_texture)
	{
		return result_e::error_value_null;
	}

	format_t gl_format = format_to_gl_format(format);
	if (0 == gl_format.internal_format)
	{
		return result_e::error_value_unexpected;
	}

	uint32_t texture = 0;
	glGenTextures(1, &texture);
	if (0 == texture)
	{
		log_line("gl: failed to create texture of size {iv2}", size.array());
		return result_e::error_gl;
	}

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, gl_format.internal_format,
		static_cast<GLsizei>(size.x), static_cast<GLsizei>(size.y),
		0, gl_format.channel,
		gl_format.data_type,
		data
	);

	// TODO: make this parameterized
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	if (should_mipmap(format))
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST
		);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	out_texture->id = static_cast<uint64_t>(texture);
	out_texture->size = size;
	out_texture->format = format;
	out_texture->log(true);
	return result_e::success;
}

void katha::gl_t::delete_texture(texture_t* texture)
{
	if ((nullptr == texture) || (0 == texture->id))
	{
		return;
	}

	log_line("gl: delete texture {u64:x}", texture->id);
	GLuint gl_texture = static_cast<GLuint>(texture->id);
	glDeleteTextures(1, &gl_texture);

	*texture = {};
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

katha::result_e katha::gl_t::create_pso(
	pso_t* out_pso,
	const vertex_layout_e vertex_layout,
	const char* vertex_shader_source,
	const char* fragment_shader_source,
	const blend_mode_e blend_mode,
	const depth_mode_e depth_mode
)
{
	if (!(out_pso && vertex_shader_source && fragment_shader_source))
	{
		return result_e::error_value_null;
	}

	if (vertex_layout_e::f3_usn2 == vertex_layout)
	{
		GLuint vao = create_vao_vertex_t(false);
		if (0 == vao)
		{
			return result_e::error_gl;
		}
		out_pso->layout = static_cast<GLuint>(vao);
	}
	else
	{
		log_line("error-gl: unknown vertex layout {u}", static_cast<uint32_t>(vertex_layout));
		return result_e::error_gl;
	}

	constexpr GLenum SHADER_TYPES[2] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
	const char* shader_sources[2] = {
		vertex_shader_source,
		fragment_shader_source
	};
	uint32_t shaders[2] = {};
	for (uint32_t i = 0; i < 2; i++)
	{
		shaders[i] = glCreateShader(SHADER_TYPES[i]);
		if (0 == shaders[i])
		{
			log_line("error-gl: failed to create shader");
			break;
		}

		// TODO: add version string based on version
		const char* sources[] = {
			shader_sources[i]
		};
		glShaderSource(shaders[i], 1, sources, nullptr);
		glCompileShader(shaders[i]);

		GLint success = GL_TRUE;
		glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &success);
		if (GL_TRUE == success)
		{
			continue;
		}
		
		// compile failed
		GLint log_length = 0;
		glGetShaderiv(shaders[i], GL_INFO_LOG_LENGTH, &log_length);
		if (log_length)
		{
			char* log = alloc<char>(log_length);
			if (log)
			{
				glGetShaderInfoLog(shaders[i], log_length, nullptr, log);
				log_line("error-gl: shader compile error: {s}", log);
				release(log);
			}
		}

		glDeleteShader(shaders[i]);
		shaders[i] = 0;
		break;
	}
	if (!(shaders[0] && shaders[1]))
	{
		if (shaders[0]) { glDeleteShader(shaders[0]); }
		if (shaders[1]) { glDeleteShader(shaders[1]); }
		GLuint vao = static_cast<GLuint>(out_pso->layout);
		glDeleteVertexArrays(1, &vao);
		return result_e::error_gl;
	}

	uint32_t program = glCreateProgram();
	if (0 == program)
	{
		log_line("error-gl: failed to create shader program");
		glDeleteShader(shaders[0]);
		glDeleteShader(shaders[1]);
		GLuint vao = static_cast<GLuint>(out_pso->layout);
		glDeleteVertexArrays(1, &vao);
		return result_e::error_gl;
	}

	glAttachShader(program, shaders[0]);
	glAttachShader(program, shaders[1]);
	glLinkProgram(program);
	
	glDeleteShader(shaders[0]);
	glDeleteShader(shaders[1]);

	GLint success = GL_TRUE;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (GL_TRUE == success)
	{
		out_pso->id = static_cast<GLuint>(program);
		out_pso->blend_mode = blend_mode;
		out_pso->depth_mode = depth_mode;

		out_pso->log(true);
		return result_e::success;
	}

	// link failed
	GLint log_length = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
	if (log_length)
	{
		char* log = alloc<char>(log_length);
		if (log)
		{
			glGetProgramInfoLog(program, log_length, nullptr, log);
			log_line("error-gl: program link error: {s}", log);
			release(log);
		}
	}
	glDeleteProgram(program);
	GLuint vao = static_cast<GLuint>(out_pso->layout);
	glDeleteVertexArrays(1, &vao);

	return result_e::error_gl;
}

void katha::gl_t::delete_pso(pso_t* pso)
{
	if ((nullptr == pso) || (0 == pso->id))
	{
		return;
	}

	if (pso->layout)
	{
		GLuint vao = static_cast<GLuint>(pso->layout);
		glDeleteVertexArrays(1, &vao);
	}

	log_line("gl: delete_pso({u64})", pso->id);
	glDeleteProgram(pso->id);
	*pso = {};
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

katha::result_e katha::gl_t::create_array_buffer(
	buffer_t* out_buffer,
	const uint32_t size,
	const void* data
)
{
	if (nullptr == out_buffer)
	{
		return result_e::error_value_null;
	}

	uint32_t buffer = 0;
	glGenBuffers(1, &buffer);
	if (0 == buffer)
	{
		log_line("error-gl: failed to create buffer of size {u}", size);
		return result_e::error_gl;
	}

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), data, GL_STATIC_DRAW);

	out_buffer->id = static_cast<uint64_t>(buffer);
	out_buffer->size = size;

	return result_e::success;
}

void katha::gl_t::delete_buffer(buffer_t* buffer)
{
	if ((nullptr == buffer) || (0 == buffer->id))
	{
		return;
	}

	GLuint gl_buffer = static_cast<GLuint>(buffer->id);
	log_line("gl: delete_buffer {u:x}", gl_buffer);
	glDeleteBuffers(1, &gl_buffer);

	*buffer = {};
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

katha::mat4 katha::gl_t::get_perspective_projection(
	const float vertical_fov_radians,
	const vec2 screen_size,
	const vec2 z_range
)
{
	const float half_fov = std::tan(vertical_fov_radians * 0.5f);
	const float aspect = aspect_xy(screen_size);
	const float f = 1.0f / half_fov;

	mat4 perspective = mat4::from_columns(
		vec4(f / aspect, 0, 0, 0),
		vec4(0, f, 0, 0),
		vec4(0, 0, -(z_range.x + z_range.y) / (z_range.y - z_range.x), -1.0f),
		vec4(0, 0, (-2.0f * z_range.x * z_range.y) / (z_range.y - z_range.x), 0)
	);
	return perspective;
}