#pragma once
#ifndef KATHA_GRAPHICS_PSO_H__
#define KATHA_GRAPHICS_PSO_H__ 1

#include "../core/enums.hpp"

namespace katha
{
	struct pso_t
	{
		uint64_t id = 0;
		uint64_t layout = 0; // GL::VetexArray, Vulkan::PipelineLayout
		
		vertex_layout_e vertex_layout = vertex_layout_e::f3_usn2;
		
		blend_mode_e blend_mode = blend_mode_e::none;
		depth_mode_e depth_mode = depth_mode_e::none;

		void log(const bool pretty_print = false) const;
	};
}

#endif