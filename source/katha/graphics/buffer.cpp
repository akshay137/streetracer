#include "buffer.hpp"
#include "../core.hpp"

void katha::buffer_t::log(const bool pretty_print) const
{
	log_line("buffer::{u64:x}, size = {u}", id, size);
}