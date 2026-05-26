#include <d2d/motion/accelerator.h>

using namespace d2d::motion;

accelerator::accelerator(
	const motion_vector& _clamp
):clamp_vector{_clamp}
{}

void accelerator::clamp(
	const motion_vector& _clamp
) {

	clamp_vector=_clamp;
}

void accelerator::accelerate_x(
	motion_vector& _target,
	t_motion _factor
) {

	away_from_zero(_target.x, _factor, clamp_vector.x);
}

void accelerator::accelerate_y(
	motion_vector& _target,
	t_motion _factor
) {

	away_from_zero(_target.y, _factor, clamp_vector.y);
}

void accelerator::accelerate(
	motion_vector& _target,
	t_motion _factor
) {

	away_from_zero(_target.x, _factor, clamp_vector.x);
	away_from_zero(_target.y, _factor, clamp_vector.y);
}

void accelerator::accelerate(
	motion_vector& _target,
	const motion_vector& _factor
) {

	away_from_zero(_target.x, _factor.x, clamp_vector.x);
	away_from_zero(_target.y, _factor.y, clamp_vector.y);
}

void accelerator::decelerate_x(
	motion_vector& _target,
	t_motion _factor
) {

	towards_zero(_target.x, _factor, clamp_vector.x);
}

void accelerator::decelerate_y(
	motion_vector& _target,
	t_motion _factor
) {

	towards_zero(_target.y, _factor, clamp_vector.y);
}

void accelerator::decelerate(
	motion_vector& _target,
	t_motion _factor
) {

	towards_zero(_target.x, _factor, clamp_vector.x);
	towards_zero(_target.y, _factor, clamp_vector.y);
}

void accelerator::decelerate(
	motion_vector& _target,
	const motion_vector& _factor
) {

	towards_zero(_target.x, _factor.x, clamp_vector.x);
	towards_zero(_target.y, _factor.y, clamp_vector.y);
}

void accelerator::away_from_zero(
	t_motion& _target,
	t_motion _factor,
	t_motion _clamp
) {

	//First, do we need to take into account the sign?
	bool is_negative=0.0==_target
		? _factor < 0.0
		: _target < 0.0;

	t_motion copy=fabs(_target);

	//Next, apply.
	copy+=fabs(_factor);
	
	//Finally, clamp.
	if(0.0 != _clamp && copy > _clamp) {

		copy=_clamp;
	}

	_target=is_negative
		? -copy
		: copy;
}

void accelerator::towards_zero(
	t_motion& _target,
	t_motion _factor,
	t_motion _clamp
) {

	if(0.0 == _target) {

		return;
	}

	bool is_negative=_target < 0.0;
	t_motion copy=fabs(_target);

	copy-=fabs(_factor);
	
	//Avoid passing to the other side!!!
	if(copy < 0.0) {

		copy=0.0;
	}
	
	//Finally, clamp.
	if(0.0 != _clamp && copy < _clamp) {

		copy=_clamp;
	}

	_target=is_negative
		? -copy
		: copy;
}
