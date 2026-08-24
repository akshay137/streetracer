#include "texture.hpp"
#include "../core.hpp"

void katha::texture_t::log(const bool pretty_print) const
{
	log_line("texture::{u64:x}, {u}x{u}, {s}",
		id, size.x, size.y,
		format_to_cstring(format)
	);
}