#include "texture.hpp"
#include "../core.hpp"

void katha::texture_t::log(const bool pretty_print) const
{
	log_line("texture::{u64:x}, size = {uv2}", id, size.array());

	if (!pretty_print)
	{
		return;
	}

	log_line("\t{s}", format_to_cstring(format));
}