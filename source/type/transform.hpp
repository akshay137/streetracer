#pragma once
#ifndef KATHA_PHYSICS_TRANSFORM_H__
#define KATHA_PHYSICS_TRANSFORM_H__ 1

#include "vector.hpp"
#include "quaternion.hpp"
#include "matrix.hpp"

namespace katha
{
	/*
	* right handed coordinate system
	* forward -> right -> up -> forward
	*/
	struct transform_t
	{
		static inline constexpr vec3 FORWARD = vec3(0, 0, 1);
		static inline constexpr vec3 UP = vec3(0, 1, 0);
		static inline constexpr vec3 RIGHT = vec3(1, 0, 0);

		quat_t orientation = quat_t::identity();
		vec3 position = {};

		mat4 calculate_world_matrix() const;
		mat4 calculate_view_matrix() const;

		transform_t offset_by(const transform_t& t) const;
		
		transform_t look_at(const vec3& point, const vec3& up = UP) const;
		transform_t look_at(const transform_t& transform, const vec3& up = UP) const
		{
			return look_at(transform.position, up);
		}

		vec3 local_to_world(const vec3 local) const;
	};
}

#endif