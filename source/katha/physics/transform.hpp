#pragma once
#ifndef KATHA_PHYSICS_TRANSFORM_H__
#define KATHA_PHYSICS_TRANSFORM_H__ 1

#include "../core/types.hpp"

namespace katha
{
	struct transform_t
	{
		quat_t orientation = quat_t::identity();
		vec3 position = {};

		mat4 calculate_view_matrix() const;

		transform_t offset_by(const transform_t& t) const;
		
		transform_t look_at(const vec3& point) const;
		transform_t look_at(const transform_t& transform) const
		{
			return look_at(transform.position);
		}
	};
}

#endif