#include "transform.hpp"
#include "../math/vector3.hpp"
#include "../math/quaternion.hpp"
#include "../math/matrix4.hpp"

katha::mat4 katha::Transform::calculateWorldMatrix() const
{
	const mat4 t = Translate(mat4(1.0f), position);
	const mat4 w = t * ToMat4(orientation);
	return w;
}

katha::mat4 katha::Transform::calculateViewMatrix() const
{
	const mat4 t = Translate(mat4(1.0f), -position);
	mat4 view = ToMat4(Inverse(orientation)) * t;
	return view;
}

katha::Transform katha::Transform::offsetBy(const Transform& t) const
{
	Transform result = {
		.orientation = orientation * t.orientation,
		.position = position + Rotate(orientation, t.position)
	};
	
	return result;
}

katha::Transform katha::Transform::lookAt(
	const vec3& point,
	const vec3& up
) const
{
	const vec3 direction = Normalize(point - position);
	const vec3 right = Normalize(Cross(up, direction));
	const vec3 new_up = Cross(direction, right);

	const float d = Dot(FORWARD, direction);
	if (d < -0.9999f)
	{
		const vec3 axis = Cross(FORWARD, direction);
		Transform result = {
			.orientation = quat(Normalize(Cross(up, FORWARD)), 0.0f),
			.position = position
		};
		return result;
	}

	const vec3 axis = Cross(FORWARD, direction);
	const quat look_orientation = Normalize(quat(axis, 1 + d));
	Transform look_transform = {
		.orientation = look_orientation,
		.position = position
	};
	return look_transform;
}

katha::vec3 katha::Transform::localToWorld(const vec3 local) const
{
	const vec3 rotated = Rotate(orientation, local);
	const vec3 shifted = position + rotated;
	return shifted;
}