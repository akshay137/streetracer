#include "transform.hpp"
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