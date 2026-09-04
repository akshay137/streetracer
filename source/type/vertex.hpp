#pragma once
#ifndef KATHA_GRAPHICS_VERTEX_H__
#define KATHA_GRAPHICS_VERTEX_H__ 1

#include "vector.hpp"

namespace katha
{
	// Default vertex for a 3D mesh
	struct Vertex
	{
		using UNormVec2 = Vector2<uint16_t>;
		using SNormVec3 = Vector3<int16_t>;

		vec3 position;
		UNormVec2 uv;
		SNormVec3 normal;

		// `v` must be in range 0, 1
		static constexpr UNormVec2 UNorm(const vec2 v)
		{
			UNormVec2 result(
				static_cast<uint16_t>(v.x * UINT16_MAX),
				static_cast<uint16_t>(v.y * UINT16_MAX)
			);
			return result;
		}

		// `v` must be in range -1, 1
		static constexpr SNormVec3 SNorm(const vec3 v)
		{
			SNormVec3 result(
				static_cast<int16_t>(v.x * INT16_MAX),
				static_cast<int16_t>(v.y * INT16_MAX),
				static_cast<int16_t>(v.z * INT16_MAX)
			);
			return result;
		}
	};
}

#endif