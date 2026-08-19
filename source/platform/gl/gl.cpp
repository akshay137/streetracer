#include "gl.hpp"
#include "../../katha/core/config.hpp"
#include "../../katha/graphics/vertex.hpp"
#include "../../katha/math/vector2.hpp"

#include <SDL2/SDL_opengles.h>
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

	// OpenXR needs Desktop GL, so ES 3.1 check is not necessary there
	if (!config.enable_xr)
	{
		const bool ES_3_1 = (version_major >= 3) && (version_minor >= 1);
		if (!ES_3_1)
		{
			log_line("error-gl: failed to create OpenGL ES 3.1 context");
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

	delete_framebuffer(left);
	delete_framebuffer(right);

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

uint32_t katha::gl_t::create_framebuffer_from_texture(const uint32_t texture_color)
{
	uint32_t framebuffer = 0;
	glGenFramebuffers(1, &framebuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		texture_color, 0
	);

	GLenum gl_result = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (GL_FRAMEBUFFER_COMPLETE != gl_result)
	{
		glDeleteFramebuffers(1, &framebuffer);
		log_line("error-gl: framebuffer is incomplete {x32}", gl_result);
		return 0;
	}

	log_line("gl: framebuffer {u}", framebuffer);
	return framebuffer;
}

void katha::gl_t::delete_framebuffer(const uint32_t framebuffer)
{
	log_line("gl: delete framebuffer {u}", framebuffer);
	glDeleteFramebuffers(1, &framebuffer);
}

void katha::gl_t::bind_framebuffer(const uint32_t framebuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

katha::gl_t::framebuffer_t katha::gl_t::create_framebuffer(
	const ivec2 size,
	const format_e color_texture_format,
	const format_e depth_texture_format
)
{
	log_line("gl: create_framebuffer("
		"size={iv2}, color_texture_format={s}, depth_texture_format={s}"
		")",
		size.array(),
		format_to_cstring(color_texture_format),
		format_to_cstring(depth_texture_format)
	);

	framebuffer_t framebuffer = {};
	framebuffer.size = size;
	framebuffer.color_texture = create_texture(size, color_texture_format);
	if (0 == framebuffer.color_texture)
	{
		return {};
	}

	if (format_e::none != depth_texture_format)
	{
		framebuffer.depth_texture = create_texture(size, depth_texture_format);
		if (0 == framebuffer.depth_texture)
		{
			delete_texture(framebuffer.color_texture);
			return {};
		}
	}

	glGenFramebuffers(1, &(framebuffer.framebuffer));
	if (0 == framebuffer.framebuffer)
	{
		delete_texture(framebuffer.color_texture);
		delete_texture(framebuffer.depth_texture);
		return {};
	}

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.framebuffer);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		framebuffer.color_texture, 0
	);
	if (framebuffer.depth_texture)
	{
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
			GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
			framebuffer.depth_texture, 0
		);
	}

	GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (GL_FRAMEBUFFER_COMPLETE != status)
	{
		delete_framebuffer(framebuffer);
		log_line("error-gl: framebuffer is not complete {x32}", status);
		return {};
	}

	log_line("gl: framebuffer {{ .fbo = {u}, .color = {u}, .depth = {u} }",
		framebuffer.framebuffer,
		framebuffer.color_texture,
		framebuffer.depth_texture
	);
	return framebuffer;
}

void katha::gl_t::delete_framebuffer(const framebuffer_t& framebuffer)
{
	if (0 == framebuffer.framebuffer)
	{
		return;
	}

	log_line("gl: deleting framebuffer {u}, color_texture {u}, depth_texture {u}",
		framebuffer.framebuffer,
		framebuffer.color_texture,
		framebuffer.depth_texture
	);

	glDeleteFramebuffers(1, &(framebuffer.framebuffer));
	delete_texture(framebuffer.color_texture);
	delete_texture(framebuffer.depth_texture);
}

void katha::gl_t::bind_framebuffer(const framebuffer_t& framebuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.framebuffer);
	glViewport(0, 0, framebuffer.size.x, framebuffer.size.y);
}

void katha::gl_t::blit_to_screen(const framebuffer_t& framebuffer,
	const ivec4& screen_rect,
	const bool filter_linear
)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer.framebuffer);
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
	glBindFramebuffer(GL_READ_FRAMEBUFFER, source.framebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target.framebuffer);

	glBlitFramebuffer(
		0, 0, source.size.x, source.size.y,
		0, 0, target.size.x, target.size.y,
		GL_COLOR_BUFFER_BIT, filter_linear ? GL_LINEAR : GL_NEAREST
	);
}

uint32_t katha::gl_t::create_texture(
	const ivec2 size,
	const format_e format,
	const void* data,
	const bool generate_mipmaps
)
{
	format_t gl_format = format_to_gl_format(format);
	if (0 == gl_format.internal_format)
	{
		return 0;
	}

	uint32_t texture = 0;
	glGenTextures(1, &texture);
	if (0 == texture)
	{
		log_line("gl: failed to create texture of size {iv2}", size.array());
		return 0;
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
	
	if (generate_mipmaps)
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

	log_line("gl: texture {u}, {iv2}", texture, size.array());
	return texture;
}

void katha::gl_t::delete_texture(const uint32_t texture)
{
	if (0 == texture)
	{
		return;
	}

	log_line("gl: delete texture {u}", texture);
	glDeleteTextures(1, &texture);
}

void katha::gl_t::bind_texture(const uint32_t texture, const uint32_t slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture);
}

uint32_t katha::gl_t::create_shader_program(
	const char* vertex_shader_source,
	const char* fragment_shader_source
)
{
	constexpr GLenum SHADER_TYPES[2] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };

	uint32_t shaders[2] = {};
	const char* shader_sources[2] = {
		vertex_shader_source,
		fragment_shader_source
	};

	for (int i = 0; i < 2; i++)
	{
		const char* shader_source[] = {
			shader_sources[i]
		};
		shaders[i] = glCreateShader(SHADER_TYPES[i]);
		glShaderSource(shaders[i], 1, shader_source, nullptr);
		glCompileShader(shaders[i]);

		int success = GL_TRUE;
		glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &success);
		if (!success)
		{
			int log_length = 0;
			glGetShaderiv(shaders[i], GL_INFO_LOG_LENGTH, &log_length);
			char* buffer = alloc<char>(log_length + 1);
			if (buffer)
			{
				glGetShaderInfoLog(shaders[i], log_length, nullptr, buffer);
				buffer[log_length] = 0;
				log_line("error-gl: shader compile failed: {s}", buffer);
				release(buffer);
			}
		
			for (int j = 0; j <= i; j++)
			{
				glDeleteShader(shaders[j]);
			}

			return 0;
		}
	}

	uint32_t program = glCreateProgram();
	glAttachShader(program, shaders[0]);
	glAttachShader(program, shaders[1]);
	glLinkProgram(program);
	
	int success = GL_TRUE;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		int log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
		char* buffer = alloc<char>(log_length + 1);
		if (buffer)
		{
			glGetProgramInfoLog(program, log_length, nullptr, buffer);
			buffer[log_length] = 0;
			log_line("error-gl: program link failed: {s}", buffer);
			release(buffer);
		}
		glDeleteProgram(program);
		program = 0;
	}

	glDeleteShader(shaders[0]);
	glDeleteShader(shaders[1]);

	if (program)
	{
		log_line("gl: program {u}", program);
	}

	return program;
}

void katha::gl_t::delete_shader_program(const uint32_t program)
{
	log_line("gl: delete_program {u}", program);
	glDeleteProgram(program);
}

void katha::gl_t::use_shader_program(const uint32_t program)
{
	glUseProgram(program);
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

uint32_t katha::gl_t::create_array_buffer(const uint32_t size, const void* data)
{
	uint32_t buffer = 0;
	glGenBuffers(1, &buffer);
	if (0 == buffer)
	{
		log_line("error-gl: failed to create buffer of size {u}", size);
		return 0;
	}

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), data, GL_STATIC_DRAW);

	return buffer;
}

void katha::gl_t::delete_buffer(const uint32_t buffer)
{
	log_line("gl: delete_buffer {u}", buffer);
	glDeleteBuffers(1, &buffer);
}

void katha::gl_t::bind_vertex_buffer(
	const uint32_t index,
	const uint32_t buffer,
	const uint32_t offset,
	const uint32_t stride
)
{
	glBindVertexBuffer(index, buffer,
		static_cast<GLintptr>(offset),
		static_cast<GLsizei>(stride)
	);
}

uint32_t katha::gl_t::create_vao(const bool bind)
{
	uint32_t vao = 0;
	glGenVertexArrays(1, &vao);
	if (0 == vao)
	{
		log_line("error-gl: failed to create vao");
		return 0;
	}

	if (bind)
	{
		glBindVertexArray(vao);
	}

	return vao;
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

void katha::gl_t::delete_vao(const uint32_t vao)
{
	log_line("gl: delete_vao {u}", vao);
	glDeleteVertexArrays(1, &vao);
}

void katha::gl_t::bind_vao(const uint32_t vao)
{
	glBindVertexArray(vao);
}

void katha::gl_t::set_vertex_attribute_float(
	const uint32_t attribute_index,
	const uint32_t binding_index,
	const uint32_t size,
	const uint32_t offset
)
{
	glVertexAttribFormat(attribute_index, size, GL_FLOAT, GL_FALSE, offset);
	glVertexAttribBinding(attribute_index, binding_index);
	glEnableVertexAttribArray(attribute_index);
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
		case format_e::rgb8:
			return {
				.internal_format = GL_RGB8,
				.channel = GL_RGB,
				.data_type = GL_UNSIGNED_BYTE
			};
		
		case format_e::rgba8:
			return {
				.internal_format = GL_RGBA8,
				.channel = GL_RGBA,
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

katha::mat4 katha::gl_t::perspective_matrix(
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