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
	struct Transform
	{
		static inline constexpr vec3 FORWARD = vec3(0, 0, 1);
		static inline constexpr vec3 UP = vec3(0, 1, 0);
		static inline constexpr vec3 RIGHT = vec3(1, 0, 0);

		quat orientation = quat::Identity();
		vec3 position = {};

		mat4 calculateWorldMatrix() const;
		mat4 calculateViewMatrix() const;

		Transform offsetBy(const Transform& t) const;
		
		Transform lookAt(const vec3& point, const vec3& up = UP) const;
		Transform lookAt(const Transform& transform, const vec3& up = UP) const
		{
			return lookAt(transform.position, up);
		}

		vec3 localToWorld(const vec3 local) const;
	};
}

#endif