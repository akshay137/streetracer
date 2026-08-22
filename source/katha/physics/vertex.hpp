#pragma once
#ifndef KATHA_GRAPHICS_VERTEX_H__
#define KATHA_GRAPHICS_VERTEX_H__ 1

#include "../core/types.hpp"

namespace katha
{
	// Default vertex for a 3D mesh
	struct vertex_t
	{
		using unorm_vec2 = vector2_t<uint16_t>;
		using snorm_vec2 = vector2_t<int16_t>;

		vec3 position;
		unorm_vec2 uv;
		// snorm_vec2 tangent;
		// snorm_vec2 bi_tangent;

		// `v` must be in range 0, 1
		static constexpr unorm_vec2 unorm(const vec2 v)
		{
			unorm_vec2 result(
				static_cast<uint16_t>(v.x * UINT16_MAX),
				static_cast<uint16_t>(v.y * UINT16_MAX)
			);
			return result;
		}

		// `v` must be in range -1, 1
		static constexpr snorm_vec2 snorm(const vec2 v)
		{
			snorm_vec2 result(
				static_cast<int16_t>(v.x * INT16_MAX),
				static_cast<int16_t>(v.y * INT16_MAX)
			);
			return result;
		}
	};
}

#endif