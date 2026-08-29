#include "string.hpp"
#include "types.hpp"
#include "../type/version.hpp"
#include "../utility.hpp"

#include <cinttypes>
#include <cstring>
#include <cstdio>

inline int32_t read_next_code(
	const char* str,
	uint16_t* out_index,
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

inline katha::base_e specifier_properties_to_base(const katha::string_t& props)
{
	// default is decimal
	if (0 == props.size)
	{
		return katha::base_e::decimal;
	}

	if ('b' == props[0])
	{
		return katha::base_e::binary;
	}
	if ('o' == props[0])
	{
		return katha::base_e::octal;
	}
	if ('x' == props[0])
	{
		return katha::base_e::hexadecimal;
	}

	// unknown, return decimal
	return katha::base_e::decimal;
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
	if (param_buffer.size)
	{
		const int32_t code = read_next_code(param_buffer.buffer, &pbuffer_index);
		if (pbuffer_index >= param_buffer.size)
		{
			param_buffer.size = 0;
			pbuffer_index = 0;
		}
		return code;
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
	if (spec_id.equals(SPEC_QUATERNION))
	{
		return parse_next_quaternion(spec_props);
	}
	if (spec_id.equals(SPEC_MAT4))
	{
		return parse_next_mat4(spec_props);
	}
	if (spec_id.equals(SPEC_TIMEDIFF))
	{
		return parse_next_timediff(spec_props);
	}
	if (spec_id.equals(SPEC_VERSION))
	{
		return parse_next_version(spec_props);
	}

	param_buffer.append("{unknown specifier: ");
	param_buffer.append(spec_id);
	param_buffer.append(" }");
	return next(); // unknown specifier, return next token
}

int32_t katha::string_format_t::parse_next_int(const string_t& props)
{
	const int32_t value = va_arg(args, int32_t);
	const base_e base = specifier_properties_to_base(props);
	param_buffer.size = int_to_string(value, param_buffer.buffer, base);
	return next();
}

int32_t katha::string_format_t::parse_next_uint(const string_t& props)
{
	const uint32_t value = va_arg(args, uint32_t);
	const base_e base = specifier_properties_to_base(props);
	param_buffer.size = uint_to_string(value, param_buffer.buffer, base);
	return next();
}

int32_t katha::string_format_t::parse_next_float(const string_t& props)
{
	const double value = va_arg(args, double);
	param_buffer.size = double_to_string(value, param_buffer.buffer);
	return next();
}

int32_t katha::string_format_t::parse_next_int64(const string_t& props)
{
	const int64_t value = va_arg(args, int64_t);
	const base_e base = specifier_properties_to_base(props);
	param_buffer.size = int64_to_string(value, param_buffer.buffer, base);
	return next();
}
int32_t katha::string_format_t::parse_next_uint64(const string_t& props)
{
	const uint64_t value = va_arg(args, uint64_t);
	const base_e base = specifier_properties_to_base(props);
	param_buffer.size = uint64_to_string(value, param_buffer.buffer, base);
	return next();
}

int32_t katha::string_format_t::parse_next_pointer(const string_t& props)
{
	const uintptr_t value = va_arg(args, uintptr_t);
	param_buffer.size = uint64_to_string(
		value,
		param_buffer.buffer,
		base_e::hexadecimal
	);
	return next();
}

int32_t katha::string_format_t::parse_next_cstring(const string_t& props)
{
	const char* str = va_arg(args, const char*);
	param_str = (nullptr != str) ? str : "{null_string}";
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
	const float* value = va_arg(args, float*);
	if (value)
	{
		param_buffer.append("(");
		param_buffer.size += float_to_string(value[0], param_buffer.tail());
		param_buffer.append(", ");
		param_buffer.size += float_to_string(value[1], param_buffer.tail());
		param_buffer.append(", ");
		param_buffer.size += float_to_string(value[2], param_buffer.tail());
		param_buffer.append(")");
	}
	return next();
}

int32_t katha::string_format_t::parse_next_ivec2(const string_t& props)
{
	const int32_t* value = va_arg(args, int32_t*);
	if (value)
	{
		param_buffer.append("(");
		param_buffer.size += int_to_string(value[0], param_buffer.tail());
		param_buffer.append(", ");
		param_buffer.size += int_to_string(value[1], param_buffer.tail());
		param_buffer.append(")");
	}

	return next();
}

int32_t katha::string_format_t::parse_next_uvec2(const string_t& props)
{
	const uint32_t* value = va_arg(args, uint32_t*);
	if (value)
	{
		param_buffer.append("(");
		param_buffer.size += uint_to_string(value[0], param_buffer.tail());
		param_buffer.append(", ");
		param_buffer.size += uint_to_string(value[1], param_buffer.tail());
		param_buffer.append(")");
	}

	return next();
}

int32_t katha::string_format_t::parse_next_vec2(const string_t& props)
{
	const float* value = va_arg(args, float*);
	if (value)
	{
		param_buffer.append("(");
		param_buffer.size += float_to_string(value[0], param_buffer.tail());
		param_buffer.append(", ");
		param_buffer.size += float_to_string(value[1], param_buffer.tail());
		param_buffer.append(")");
	}

	return next();
}

int32_t katha::string_format_t::parse_next_quaternion(const string_t& props)
{
	const float* q = va_arg(args, float*);
	if (q)
	{
		param_buffer.size = 0;
		param_buffer.append("quat: ");
		param_buffer.size += float_to_string(q[3], param_buffer.tail());
		param_buffer.append(" (");
		param_buffer.size += float_to_string(q[0], param_buffer.tail());
		param_buffer.append(", ");
		param_buffer.size += float_to_string(q[1], param_buffer.tail());
		param_buffer.append(", ");
		param_buffer.size += float_to_string(q[2], param_buffer.tail());
		param_buffer.append(")");
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
			param_buffer.append("mat4-rm: [ [");
			param_buffer.size += float_to_string(m[0], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += float_to_string(m[4], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += float_to_string(m[8], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += float_to_string(m[12], param_buffer.tail());
			param_buffer.append("], [ ");
			param_buffer.size += float_to_string(m[1], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += float_to_string(m[5], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += float_to_string(m[9], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += float_to_string(m[13], param_buffer.tail());
			param_buffer.append("], [ ");
			param_buffer.size += float_to_string(m[2], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += float_to_string(m[6], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += float_to_string(m[10], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += float_to_string(m[14], param_buffer.tail());
			param_buffer.append("], [ ");
			param_buffer.size += float_to_string(m[3], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += float_to_string(m[7], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += float_to_string(m[11], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += float_to_string(m[15], param_buffer.tail());
			param_buffer.append("] ]");
		}
		else
		{
			param_buffer.append("mat4-cm: [ [");
			param_buffer.size += float_to_string(m[0], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += float_to_string(m[1], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += float_to_string(m[2], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += float_to_string(m[3], param_buffer.tail());
			param_buffer.append("], [ ");
			param_buffer.size += float_to_string(m[4], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += float_to_string(m[5], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += float_to_string(m[6], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += float_to_string(m[7], param_buffer.tail());
			param_buffer.append("], [ ");
			param_buffer.size += float_to_string(m[8], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += float_to_string(m[9], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += float_to_string(m[10], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += float_to_string(m[11], param_buffer.tail());
			param_buffer.append("], [ ");
			param_buffer.size += float_to_string(m[12], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += float_to_string(m[13], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += float_to_string(m[14], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += float_to_string(m[15], param_buffer.tail());
			param_buffer.append("] ]");
		}
	}

	return next();
}

int32_t katha::string_format_t::parse_next_timediff(const string_t& props)
{
	const uint64_t value = va_arg(args, uint64_t);

	if (value < 1000ull) // nanoseconds
	{
		param_buffer.size = uint64_to_string(value, param_buffer.buffer);
		param_buffer.append(" ns");
	}
	else if (value < 1000000ull) // microseconds
	{
		param_buffer.size = float_to_string(
			value / 1000.0f,
			param_buffer.buffer
		);
		param_buffer.append(" \xce\xbcs");
	}
	else if (value < 1000000000ull) // milliseconds
	{
		param_buffer.size = float_to_string(
			value / 1000000.0f,
			param_buffer.buffer
		);
		param_buffer.append(" ms");
	}
	else if (value < 1000000000000ull) // seconds
	{
		param_buffer.size = float_to_string(
			value / 1000000000.0,
			param_buffer.buffer
		);
		param_buffer.append(" s");
	}
	else // just log raw value at this point
	{
		param_buffer.size = uint64_to_string(
			value,
			param_buffer.buffer
		);
		param_buffer.append(" ns");
	}

	return next();
}

int32_t katha::string_format_t::parse_next_version(const string_t& props)
{
	const version_t* version = va_arg(args, version_t*);
	if (version)
	{
		param_buffer.size = uint_to_string(version->major, param_buffer.buffer);
		param_buffer.append(".");
		param_buffer.size += uint_to_string(version->minor, param_buffer.tail());
		param_buffer.append(".");
		param_buffer.size += uint_to_string(version->patch, param_buffer.tail());
	}
	return next();
}