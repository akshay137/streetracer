#include "texture.hpp"
#include "../utility.hpp"

void katha::texture_t::log(const char* prefix) const
{
	log_line("texture: {s} {u64:x} {u}x{u} {s}",
		prefix ? prefix : "",
		handle,
		size.x, size.y,
		format_to_cstring(format)
	);
}