#include "transform.hpp"
#include "../math/vector3.hpp"
#include "../math/quaternion.hpp"
#include "../math/matrix4.hpp"

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

katha::transform_t katha::transform_t::look_at(const vec3& point) const
{
	constexpr vec3 FORWARD = vec3(0, 0, -1);
	
	const vec3 direction = normalize(point - position);
	const vec3 axis = cross(FORWARD, direction);
	const float w = 1 + dot(FORWARD, direction);

	const quat_t look_orientation = normalize(quat_t(axis, w));
	transform_t look_transform = {
		.orientation = look_orientation,
		.position = position
	};
	return look_transform;
}