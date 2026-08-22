#include "pso.hpp"
#include "../core.hpp"

void katha::pso_t::log(const bool pretty_print) const
{
	log_line("pso::{u64:x}, .layout {u64:x}", id, layout);
	
	if (!pretty_print)
	{
		return;
	}

	log_line("\t{s}", vertex_layout_to_cstring(vertex_layout));
	log_line("\t{s}", blend_mode_to_cstring(blend_mode));
	log_line("\t{s}", depth_mode_to_cstring(depth_mode));
}