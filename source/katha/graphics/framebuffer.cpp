#include "framebuffer.hpp"
#include "../core.hpp"

void katha::framebuffer_t::log(const bool pretty_print) const
{
	log_line("framebuffer::{u64:x}, .size {uv2}", id, size.array());

	if (!pretty_print)
	{
		return;
	}

	log_line("\ttextures.color_0 {u64:x} {s}",
		color_0.id,
		format_to_cstring(color_0.format)
	);
	log_line("\ttextures.depth_stencil {u64:x} {s}",
		depth_stencil.id,
		format_to_cstring(depth_stencil.format)
	);
}