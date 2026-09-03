#include "string.hpp"
#include "types.hpp"
#include "../type/version.hpp"
#include "../utility.hpp"

#include <cinttypes>
#include <cstring>
#include <cstdio>

inline int32_t ReadNextCode(
	const char* str,
	uint16_t* out_index,
	uint32_t* out_bytes_read = nullptr
)
{
	uint32_t bytes_read = 0;
	const int32_t code = katha::String::ReadUTF8(str + *out_index, &bytes_read);
	*out_index += bytes_read;
	katha::WriteChecked<uint32_t>(out_bytes_read, bytes_read);
	return code;
}

inline katha::Base SpecifierPropertiesToBase(const katha::String& props)
{
	// default is decimal
	if (0 == props.size) {
		return katha::Base::DECIMAL;
	}

	if ('b' == props[0]) {
		return katha::Base::BINARY;
	}
	if ('o' == props[0]) {
		return katha::Base::OCTAL;
	}
	if ('x' == props[0]) {
		return katha::Base::HEXADECIMAL;
	}

	// unknown, return decimal
	return katha::Base::DECIMAL;
}

int32_t katha::StringFormat::next()
{
	if (param_str)
	{
		const int32_t code = ReadNextCode(param_str, &pbuffer_index);
		if (code)
		{
			return code;
		}

		param_str = nullptr;
		pbuffer_index = 0;
	}
	if (param_buffer.size)
	{
		const int32_t code = ReadNextCode(param_buffer.buffer, &pbuffer_index);
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

	int32_t code = ReadNextCode(format.buffer, &format_index);
	if (0 == code)
	{
		return 0;
	}
	if ('{' != code)
	{
		return code;
	}

	const uint32_t spec_start = format_index;
	code = ReadNextCode(format.buffer, &format_index);

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
		code = ReadNextCode(format.buffer, &format_index, &bytes_read);
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
	const String spec_id(
		format.buffer + spec_start,
		spec_id_length
	);
	const String spec_props(
		format.buffer + prop_start,
		prop_start ? (full_spec_length - spec_id_length - 1) : 0
	);

	if (spec_id.equals(SPEC_INT))
	{
		return __parseNextInt(spec_props);
	}
	if (spec_id.equals(SPEC_UINT))
	{
		return __parseNextUInt(spec_props);
	}
	if (spec_id.equals(SPEC_FLOAT))
	{
		return __parseNextFloat(spec_props);
	}
	if (spec_id.equals(SPEC_INT64))
	{
		return __parseNextInt64(spec_props);
	}
	if (spec_id.equals(SPEC_UINT64))
	{
		return __parseNextUInt64(spec_props);
	}
	if (spec_id.equals(SPEC_POINTER))
	{
		return __parseNextPointer(spec_props);
	}
	if (spec_id.equals(SPEC_CSTRING))
	{
		return __parseNextCString(spec_props);
	}
	if (spec_id.equals(SPEC_BOOL))
	{
		return __parseNextBool(spec_props);
	}
	if (spec_id.equals(SPEC_VEC3))
	{
		return __parseNextVec3(spec_props);
	}
	if (spec_id.equals(SPEC_IVEC2))
	{
		return __parseNextIVec2(spec_props);
	}
	if (spec_id.equals(SPEC_UVEC2))
	{
		return __parseNextUVec2(spec_props);
	}
	if (spec_id.equals(SPEC_VEC2))
	{
		return __parseNextVec2(spec_props);
	}
	if (spec_id.equals(SPEC_QUATERNION))
	{
		return __parseNextQuaternion(spec_props);
	}
	if (spec_id.equals(SPEC_MAT4))
	{
		return __parseNextMat4(spec_props);
	}
	if (spec_id.equals(SPEC_TIMEDIFF))
	{
		return __parseNextTimeDiff(spec_props);
	}
	if (spec_id.equals(SPEC_VERSION))
	{
		return __parseNextVersion(spec_props);
	}
	if (spec_id.equals(SPEC_SOURCE))
	{
		return __parseNextSource(spec_props);
	}

	param_buffer.append("{unknown specifier: ");
	param_buffer.append(spec_id);
	param_buffer.append(" }");
	return next(); // unknown specifier, return next token
}

int32_t katha::StringFormat::__parseNextInt(const String& props)
{
	const int32_t value = va_arg(args, int32_t);
	const Base base = SpecifierPropertiesToBase(props);
	param_buffer.size = IntToString(value, param_buffer.buffer, base);
	return next();
}

int32_t katha::StringFormat::__parseNextUInt(const String& props)
{
	const uint32_t value = va_arg(args, uint32_t);
	const Base base = SpecifierPropertiesToBase(props);
	param_buffer.size = UIntToString(value, param_buffer.buffer, base);
	return next();
}

int32_t katha::StringFormat::__parseNextFloat(const String& props)
{
	const double value = va_arg(args, double);
	param_buffer.size = DoubleToString(value, param_buffer.buffer);
	return next();
}

int32_t katha::StringFormat::__parseNextInt64(const String& props)
{
	const int64_t value = va_arg(args, int64_t);
	const Base base = SpecifierPropertiesToBase(props);
	param_buffer.size = Int64ToString(value, param_buffer.buffer, base);
	return next();
}
int32_t katha::StringFormat::__parseNextUInt64(const String& props)
{
	const uint64_t value = va_arg(args, uint64_t);
	const Base base = SpecifierPropertiesToBase(props);
	param_buffer.size = UInt64ToString(value, param_buffer.buffer, base);
	return next();
}

int32_t katha::StringFormat::__parseNextPointer(const String& props)
{
	const uintptr_t value = va_arg(args, uintptr_t);
	param_buffer.size = UInt64ToString(
		value,
		param_buffer.buffer,
		Base::HEXADECIMAL
	);
	return next();
}

int32_t katha::StringFormat::__parseNextCString(const String& props)
{
	const char* str = va_arg(args, const char*);
	param_str = (nullptr != str) ? str : "{null_string}";
	return next();
}

int32_t katha::StringFormat::__parseNextBool(const String& props)
{
	const int32_t value = va_arg(args, int32_t);
	param_str = (value != 0) ? "true" : "false";
	return next();
}

int32_t katha::StringFormat::__parseNextVec3(const String& props)
{
	const float* value = va_arg(args, float*);
	if (value)
	{
		param_buffer.append("(");
		param_buffer.size += FloatToString(value[0], param_buffer.tail());
		param_buffer.append(", ");
		param_buffer.size += FloatToString(value[1], param_buffer.tail());
		param_buffer.append(", ");
		param_buffer.size += FloatToString(value[2], param_buffer.tail());
		param_buffer.append(")");
	}
	return next();
}

int32_t katha::StringFormat::__parseNextIVec2(const String& props)
{
	const int32_t* value = va_arg(args, int32_t*);
	if (value)
	{
		param_buffer.append("(");
		param_buffer.size += IntToString(value[0], param_buffer.tail());
		param_buffer.append(", ");
		param_buffer.size += IntToString(value[1], param_buffer.tail());
		param_buffer.append(")");
	}

	return next();
}

int32_t katha::StringFormat::__parseNextUVec2(const String& props)
{
	const uint32_t* value = va_arg(args, uint32_t*);
	if (value)
	{
		param_buffer.append("(");
		param_buffer.size += UIntToString(value[0], param_buffer.tail());
		param_buffer.append(", ");
		param_buffer.size += UIntToString(value[1], param_buffer.tail());
		param_buffer.append(")");
	}

	return next();
}

int32_t katha::StringFormat::__parseNextVec2(const String& props)
{
	const float* value = va_arg(args, float*);
	if (value)
	{
		param_buffer.append("(");
		param_buffer.size += FloatToString(value[0], param_buffer.tail());
		param_buffer.append(", ");
		param_buffer.size += FloatToString(value[1], param_buffer.tail());
		param_buffer.append(")");
	}

	return next();
}

int32_t katha::StringFormat::__parseNextQuaternion(const String& props)
{
	const float* q = va_arg(args, float*);
	if (q)
	{
		param_buffer.size = 0;
		param_buffer.append("quat: ");
		param_buffer.size += FloatToString(q[3], param_buffer.tail());
		param_buffer.append(" (");
		param_buffer.size += FloatToString(q[0], param_buffer.tail());
		param_buffer.append(", ");
		param_buffer.size += FloatToString(q[1], param_buffer.tail());
		param_buffer.append(", ");
		param_buffer.size += FloatToString(q[2], param_buffer.tail());
		param_buffer.append(")");
	}
	return next();
}

int32_t katha::StringFormat::__parseNextMat4(const String& props)
{
	const float* m = va_arg(args, float*);
	if (m)
	{
		const bool row_major = props.size && props[0] == 'r';
		if (row_major)
		{
			param_buffer.append("mat4-rm: [ [");
			param_buffer.size += FloatToString(m[0], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += FloatToString(m[4], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += FloatToString(m[8], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += FloatToString(m[12], param_buffer.tail());
			param_buffer.append("], [ ");
			param_buffer.size += FloatToString(m[1], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += FloatToString(m[5], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += FloatToString(m[9], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += FloatToString(m[13], param_buffer.tail());
			param_buffer.append("], [ ");
			param_buffer.size += FloatToString(m[2], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += FloatToString(m[6], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += FloatToString(m[10], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += FloatToString(m[14], param_buffer.tail());
			param_buffer.append("], [ ");
			param_buffer.size += FloatToString(m[3], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += FloatToString(m[7], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += FloatToString(m[11], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += FloatToString(m[15], param_buffer.tail());
			param_buffer.append("] ]");
		}
		else
		{
			param_buffer.append("mat4-cm: [ [");
			param_buffer.size += FloatToString(m[0], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += FloatToString(m[1], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += FloatToString(m[2], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += FloatToString(m[3], param_buffer.tail());
			param_buffer.append("], [ ");
			param_buffer.size += FloatToString(m[4], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += FloatToString(m[5], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += FloatToString(m[6], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += FloatToString(m[7], param_buffer.tail());
			param_buffer.append("], [ ");
			param_buffer.size += FloatToString(m[8], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += FloatToString(m[9], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += FloatToString(m[10], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += FloatToString(m[11], param_buffer.tail());
			param_buffer.append("], [ ");
			param_buffer.size += FloatToString(m[12], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += FloatToString(m[13], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += FloatToString(m[14], param_buffer.tail());
			param_buffer.append(", ");
			param_buffer.size += FloatToString(m[15], param_buffer.tail());
			param_buffer.append("] ]");
		}
	}

	return next();
}

int32_t katha::StringFormat::__parseNextTimeDiff(const String& props)
{
	const uint64_t value = va_arg(args, uint64_t);

	if (value < 1000ull) // nanoseconds
	{
		param_buffer.size = UInt64ToString(value, param_buffer.buffer);
		param_buffer.append(" ns");
	}
	else if (value < 1000000ull) // microseconds
	{
		param_buffer.size = FloatToString(
			value / 1000.0f,
			param_buffer.buffer
		);
		param_buffer.append(" \xce\xbcs");
	}
	else if (value < 1000000000ull) // milliseconds
	{
		param_buffer.size = FloatToString(
			value / 1000000.0f,
			param_buffer.buffer
		);
		param_buffer.append(" ms");
	}
	else if (value < 1000000000000ull) // seconds
	{
		param_buffer.size = FloatToString(
			value / 1000000000.0,
			param_buffer.buffer
		);
		param_buffer.append(" s");
	}
	else // just log raw value at this point
	{
		param_buffer.size = UInt64ToString(
			value,
			param_buffer.buffer
		);
		param_buffer.append(" ns");
	}

	return next();
}

int32_t katha::StringFormat::__parseNextVersion(const String& props)
{
	const Version* version = va_arg(args, Version*);
	if (version)
	{
		param_buffer.size = UIntToString(version->major, param_buffer.buffer);
		param_buffer.append(".");
		param_buffer.size += UIntToString(version->minor, param_buffer.tail());
		param_buffer.append(".");
		param_buffer.size += UIntToString(version->patch, param_buffer.tail());
	}
	return next();
}

int32_t katha::StringFormat::__parseNextSource(const String& props)
{
	source_t* source = va_arg(args, source_t*);
	if (source)
	{
		const char* filename = source->file_name();
		const uint32_t line = static_cast<uint32_t>(source->line());

		// FIXME
		static uint32_t filename_offset = [](const char* filename) {
			if (('/' == filename[0]))
			{
				// linux absolute path, we need path from `source/`
				uint32_t offset = 0;
				if (String(filename).find("source/", &offset))
				{
					return offset + 7u;
				}
			}

			return 0u;
		}(filename);
		
		param_buffer.size = 0;
		param_buffer.append(source->function_name());
		param_buffer.append("; ");
		param_buffer.append(filename + filename_offset);
		param_buffer.append(":");
		param_buffer.size += UIntToString(line, param_buffer.tail());
	}
	return next();
}