#include "app/projectile_generator.h"
#include <iostream>

using namespace app;

projectile_generator::projectile_generator(
	d2d::collision::box _box,
	types _type,
	int _velocity,
	int _tag,
	int _volley,
	int _pause_time_ms,
	int _rest_time_ms,
	bool _active
):
	state{projectile_generator::states::volley},
	active{_active},
	type{_type},
	tag{_tag},
	volley_total{_volley},
	pr_data{
		{(double)_velocity, 0.0},
		_box
	}
{
	float pause_time=_pause_time_ms / 1000;
	float rest_time=_rest_time_ms / 1000;
	
	//All timeouts start paused.
	timeouts.add(timeout_volley, pause_time, pause_time, true);
	timeouts.add(timeout_rest, rest_time, rest_time, true);

	if(active) {

		timeouts.resume(timeout_volley);
	}
}

bool projectile_generator::tic(
	float _delta
) {

	if(!active) {

		return false;
	}

	timeouts.tic(_delta);

	switch(state) {

		case states::volley:

			if(timeouts.is_expired(timeout_volley)) {

				++volley_count;
				timeouts.reset(timeout_volley);

				if(volley_total==volley_count) {

					volley_count=0;
					timeouts.pause(timeout_volley)
						.reset(timeout_rest)
						.resume(timeout_rest);
					state=states::rest;
				}

				return true;
			}
		break;
		case states::rest:

			if(timeouts.is_expired(timeout_rest)) {

				timeouts.pause(timeout_rest)
					.reset(timeout_volley)
					.resume(timeout_volley);
				state=states::volley;
			}
		break;
	}

	return false;
}

void projectile_generator::activate() {

	active=true;
	volley_count=0;
	state=states::volley;
	timeouts.reset(timeout_volley).reset(timeout_volley);
}

void projectile_generator::deactivate() {

	active=false;
}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const app::projectile_generator&
) {

	_stream<<"projectile generator[]";
	return _stream;
}
