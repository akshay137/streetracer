#include "string.hpp"
#include "../core/types.hpp"

#include <cinttypes>
#include <cstdio>

inline int32_t read_next_code(
	const char* str,
	uint32_t* out_index,
	uint32_t* out_bytes_read = nullptr
)
{
	uint32_t bytes_read = 0;
	const int32_t code = katha::string_t::read_utf8(
		str + *out_index,
		&bytes_read
	);
	*out_index += bytes_read;
	katha::write_checked<uint32_t>(out_bytes_read, bytes_read);
	return code;
}

int32_t katha::string_format_t::next()
{
	if (param_str)
	{
		const int32_t code = read_next_code(param_str, &pbuffer_index);
		if (code)
		{
			return code;
		}

		param_str = nullptr;
		pbuffer_index = 0;
	}
	if (pbuffer_length)
	{
		const int32_t code = read_next_code(param_buffer, &pbuffer_index);
		if (code)
		{
			return code;
		}

		pbuffer_length = 0;
		pbuffer_index = 0;
	}

	if (format_index >= format.size)
	{
		// end of format string
		return 0;
	}

	int32_t code = read_next_code(format.buffer, &format_index);
	if (0 == code)
	{
		return 0;
	}
	if ('{' != code)
	{
		return code;
	}

	const uint32_t spec_start = format_index;
	code = read_next_code(format.buffer, &format_index);

	if ((0 == code) || ('{' == code))
	{
		return code;
	}
	if ('}' == code) // empty specifier, move forward
	{
		return next();
	}

	uint32_t bytes_read = 0;
	uint32_t prop_start = 0;
	while ('}' != code)
	{
		code = read_next_code(format.buffer, &format_index, &bytes_read);
		if ((0 == code) || (format_index > format.size))
		{
			return 0;
		}
		if ((':' == code) && (0 == prop_start))
		{
			prop_start = format_index;
		}
	}

	const uint32_t full_spec_length = (format_index - bytes_read) - spec_start;
	uint32_t spec_id_length = full_spec_length;
	uint32_t prop_length = 0;
	if (prop_start)
	{
		spec_id_length = prop_start - spec_start - 1;
		prop_length = full_spec_length - spec_id_length - 1;
	}
	const string_t spec_id(
		format.buffer + spec_start,
		spec_id_length
	);
	const string_t spec_props(
		format.buffer + prop_start,
		prop_start ? (full_spec_length - spec_id_length - 1) : 0
	);
	if (spec_id.equals(SPEC_INT))
	{
		return parse_next_int(spec_props);
	}
	if (spec_id.equals(SPEC_UINT))
	{
		return parse_next_uint(spec_props);
	}
	if (spec_id.equals(SPEC_FLOAT))
	{
		return parse_next_float(spec_props);
	}
	if (spec_id.equals(SPEC_INT64))
	{
		return parse_next_int64(spec_props);
	}
	if (spec_id.equals(SPEC_UINT64))
	{
		return parse_next_uint64(spec_props);
	}
	if (spec_id.equals(SPEC_POINTER))
	{
		return parse_next_pointer(spec_props);
	}
	if (spec_id.equals(SPEC_CSTRING))
	{
		return parse_next_cstring(spec_props);
	}
	if (spec_id.equals(SPEC_BOOL))
	{
		return parse_next_bool(spec_props);
	}
	if (spec_id.equals(SPEC_VEC3))
	{
		return parse_next_vec3(spec_props);
	}
	if (spec_id.equals(SPEC_IVEC2))
	{
		return parse_next_ivec2(spec_props);
	}
	if (spec_id.equals(SPEC_UVEC2))
	{
		return parse_next_uvec2(spec_props);
	}
	if (spec_id.equals(SPEC_VEC2))
	{
		return parse_next_vec2(spec_props);
	}
	if (spec_id.equals(SPEC_MAT4))
	{
		return parse_next_mat4(spec_props);
	}
	if (spec_id.equals(SPEC_TIMEDIFF))
	{
		return parse_next_timediff(spec_props);
	}
	if (spec_id.equals(SPEC_HEX32))
	{
		return parse_next_hex32(spec_props);
	}
	if (spec_id.equals(SPEC_HEX64))
	{
		return parse_next_hex64(spec_props);
	}
	
	pbuffer_length = snprintf(param_buffer, PARAM_BUFFER_SIZE,
		"{unknown specifier: %.*s}",
		spec_id.size, spec_id.buffer
	);
	return next(); // unknown specifier, return next token
}

int32_t katha::string_format_t::parse_next_int(const string_t& props)
{
	const int32_t value = va_arg(args, int32_t);
	pbuffer_length = snprintf(param_buffer, PARAM_BUFFER_SIZE,
		"%" PRIi32, value
	);
	return next();
}

int32_t katha::string_format_t::parse_next_uint(const string_t& props)
{
	const uint32_t value = va_arg(args, uint32_t);
	pbuffer_length = snprintf(param_buffer, PARAM_BUFFER_SIZE,
		"%" PRIu32, value
	);
	return next();
}

int32_t katha::string_format_t::parse_next_float(const string_t& props)
{
	const double value = va_arg(args, double);
	pbuffer_length = snprintf(param_buffer, PARAM_BUFFER_SIZE,
		"%g", value
	);
	return next();
}

int32_t katha::string_format_t::parse_next_int64(const string_t& props)
{
	const int64_t value = va_arg(args, int64_t);
	pbuffer_length = snprintf(param_buffer, PARAM_BUFFER_SIZE,
		"%" PRIi64, value
	);
	return next();
}
int32_t katha::string_format_t::parse_next_uint64(const string_t& props)
{
	const uint64_t value = va_arg(args, uint64_t);
	pbuffer_length = snprintf(param_buffer, PARAM_BUFFER_SIZE,
		"%" PRIu64, value
	);
	return next();
}

int32_t katha::string_format_t::parse_next_pointer(const string_t& props)
{
	const uintptr_t value = va_arg(args, uintptr_t);
	pbuffer_length = snprintf(param_buffer, PARAM_BUFFER_SIZE,
		"ptr:%" PRIxPTR, value
	);
	return next();
}

int32_t katha::string_format_t::parse_next_cstring(const string_t& props)
{
	const char* str = va_arg(args, const char*);
	if (nullptr == str)
	{
		param_str = "{null_string}";
	}
	else
	{
		param_str = str;
	}
	return next();
}

int32_t katha::string_format_t::parse_next_bool(const string_t& props)
{
	const int32_t value = va_arg(args, int32_t);
	param_str = (value != 0) ? "true" : "false";
	return next();
}

int32_t katha::string_format_t::parse_next_vec3(const string_t& props)
{
	const vec3* value = va_arg(args, vec3*);
	if (value)
	{
		pbuffer_length = snprintf(param_buffer, PARAM_BUFFER_SIZE,
			"{ .x = %g, .y = %g, .z = %g }",
			value->x, value->y, value->z
		);
	}
	return next();
}

int32_t katha::string_format_t::parse_next_ivec2(const string_t& props)
{
	const ivec2* value = va_arg(args, ivec2*);
	if (value)
	{
		pbuffer_length = snprintf(param_buffer, PARAM_BUFFER_SIZE,
			"{ .x = %" PRIi32 ", .y = %" PRIi32 " }",
			value->x, value->y
		);
	}

	return next();
}

int32_t katha::string_format_t::parse_next_uvec2(const string_t& props)
{
	const uvec2* value = va_arg(args, uvec2*);
	if (value)
	{
		pbuffer_length = snprintf(param_buffer, PARAM_BUFFER_SIZE,
			"{ .x = %" PRIu32 ", .y = %" PRIu32 " }",
			value->x, value->y
		);
	}

	return next();
}

int32_t katha::string_format_t::parse_next_vec2(const string_t& props)
{
	const vec2* value = va_arg(args, vec2*);
	if (value)
	{
		pbuffer_length = snprintf(param_buffer, PARAM_BUFFER_SIZE,
			"{ .x = %g, .y = %g  }",
			value->x, value->y
		);
	}

	return next();
}

int32_t katha::string_format_t::parse_next_mat4(const string_t& props)
{
	const float* m = va_arg(args, float*);
	if (m)
	{
		const bool row_major = props.size && props[0] == 'r';
		if (row_major)
		{
			pbuffer_length = snprintf(param_buffer, PARAM_BUFFER_SIZE,
				"mat4-rm: [ [ %g, %g, %g, %g ], [ %g, %g, %g, %g ], [ %g, %g, %g, %g ], [ %g, %g, %g, %g ] ]",
				m[0], m[4], m[8], m[12],
				m[1], m[5], m[9], m[13],
				m[2], m[6], m[10], m[14],
				m[3], m[7], m[11], m[15]
			);
		}
		else
		{
			pbuffer_length = snprintf(param_buffer, PARAM_BUFFER_SIZE,
				"mat4-cm: [ [ %g, %g, %g, %g ], [ %g, %g, %g, %g ], [ %g, %g, %g, %g ], [ %g, %g, %g, %g ] ]",
				m[0], m[1], m[2], m[3],
				m[4], m[5], m[6], m[7],
				m[8], m[9], m[10], m[11],
				m[12], m[13], m[14], m[15]
			);
		}
	}

	return next();
}

int32_t katha::string_format_t::parse_next_timediff(const string_t& props)
{
	const uint64_t value = va_arg(args, uint64_t);
	if (value < 1000ull) // nanoseconds
	{
		pbuffer_length = snprintf(param_buffer, PARAM_BUFFER_SIZE,
			"%" PRIu64 " ns", value
		);
	}
	else if (value < 1000000ull) // microseconds
	{
		pbuffer_length = snprintf(param_buffer, PARAM_BUFFER_SIZE,
			"%.3f \xce\xbcs", value / 1000.0
		);
	}
	else if (value < 1000000000ull) // milliseconds
	{
		pbuffer_length = snprintf(param_buffer, PARAM_BUFFER_SIZE,
			"%.3f ms", value / 1000000.0
		);
	}
	else if (value < 1000000000000ull) // seconds
	{
		pbuffer_length = snprintf(param_buffer, PARAM_BUFFER_SIZE,
			"%.3f s", value / 1000000000.0
		);
	}
	else // just log raw value at this point
	{
		pbuffer_length = snprintf(param_buffer, PARAM_BUFFER_SIZE,
			"%" PRIu64 " ns", value
		);
	}

	return next();
}

int32_t katha::string_format_t::parse_next_hex32(const string_t& props)
{
	const uint32_t value = va_arg(args, uint32_t);
	pbuffer_length = snprintf(param_buffer, PARAM_BUFFER_SIZE,
		"hex:%" PRIX32, value
	);
	return next();
}

int32_t katha::string_format_t::parse_next_hex64(const string_t& props)
{
	const uint64_t value = va_arg(args, uint64_t);
	pbuffer_length = snprintf(param_buffer, PARAM_BUFFER_SIZE,
		"hex:%" PRIX64, value
	);
	return next();
}