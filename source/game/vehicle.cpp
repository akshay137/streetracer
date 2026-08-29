#include "vehicle.hpp"
#include "../math/vector3.hpp"
#include "../math/quaternion.hpp"

void katha::vehicle_t::apply(
	const float throttle,
	const float brake,
	const float steering_angle,
	const float delta
)
{
	const float final_velocity = 10 * (throttle - brake);
	if (abs(final_velocity) < 0.05)
	{
		// no movement
		// return;
	}

	constexpr float TURN_RATE = 90.0f;
	constexpr float HALF_TURN_RATE = TURN_RATE * 0.5f;
	const float s = std::sin(radians(steering_angle * HALF_TURN_RATE * delta));
	const float c = std::cos(radians(steering_angle * HALF_TURN_RATE * delta));

	const quat_t steer_q = quat_t(-transform_t::UP * s, c);
	quat_t orientation = normalize(transform.orientation * steer_q); // local

	const vec3 forward = rotate(orientation, transform_t::FORWARD);

	transform.position += forward * final_velocity * delta;
	transform.orientation = orientation;
}