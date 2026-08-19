#include "gamepad.hpp"
#include "../katha/math/vector2.hpp"

void katha::gamepad_t::clamp()
{
	stick_left = katha::clamp(stick_left, -1.0f, 1.0f);
	stick_right = katha::clamp(stick_right, -1.0f, 1.0f);

	trigger_left = katha::clamp(trigger_left, 0.0f, 1.0f);
	trigger_right = katha::clamp(trigger_right, 0.0f, 1.0f);
}

void katha::gamepad_t::merge(const gamepad_t& gp)
{
	stick_left += gp.stick_left;
	stick_right += gp.stick_right;

	trigger_left += gp.trigger_left;
	trigger_right += gp.trigger_right;

	dpad_up = dpad_up || gp.dpad_up;
	dpad_left = dpad_left || gp.dpad_left;
	dpad_down = dpad_down || gp.dpad_down;
	dpad_right = dpad_right || gp.dpad_right;

	action_up = action_up || gp.action_up;
	action_left = action_left || gp.action_left;
	action_down = action_down || gp.action_down;
	action_right = action_right || gp.action_right;

	shoulder_left = shoulder_left || gp.shoulder_left;
	shoulder_right = shoulder_right || gp.shoulder_right;

	l3 = l3 || gp.l3;
	r3 = r3 || gp.r3;

	select = select || gp.select;
	start = start || gp.start;

	clamp();
}