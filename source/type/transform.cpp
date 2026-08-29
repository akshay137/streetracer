#include "transform.hpp"
#include "../math/vector3.hpp"
#include "../math/quaternion.hpp"
#include "../math/matrix4.hpp"

katha::mat4 katha::transform_t::calculate_world_matrix() const
{
	const mat4 t = translate(mat4(1.0f), position);
	const mat4 w = t * to_mat4(orientation);
	return w;
}

katha::mat4 katha::transform_t::calculate_view_matrix() const
{
	const mat4 t = translate(mat4(1.0f), -position);
	mat4 view = to_mat4(inverse(orientation)) * t;
	return view;
}

katha::transform_t katha::transform_t::offset_by(const transform_t& t) const
{
	transform_t result = {
		.orientation = orientation * t.orientation,
		.position = position + rotate(orientation, t.position)
	};
	
	return result;
}

katha::transform_t katha::transform_t::look_at(
	const vec3& point,
	const vec3& up
) const
{
	const vec3 direction = normalize(point - position);
	const vec3 right = normalize(cross(up, direction));
	const vec3 new_up = cross(direction, right);

	const float d = dot(FORWARD, direction);
	if (d < -0.9999f)
	{
		const vec3 axis = cross(FORWARD, direction);
		transform_t result = {
			.orientation = quat_t(normalize(cross(up, FORWARD)), 0.0f),
			.position = position
		};
		return result;
	}

	const vec3 axis = cross(FORWARD, direction);
	const quat_t look_orientation = normalize(quat_t(axis, 1 + d));
	transform_t look_transform = {
		.orientation = look_orientation,
		.position = position
	};
	return look_transform;
}

katha::vec3 katha::transform_t::local_to_world(const vec3 local) const
{
	const vec3 rotated = rotate(orientation, local);
	const vec3 shifted = position + rotated;
	return shifted;
}