#include "app/projectile_generator.h"
#include <iostream>

using namespace app;

projectile_generator::projectile_generator(
	d2d::collision::point _point,
	types _type,
	int _velocity,
	int _tag,
	int _volley,
	int _pause_time_ms,
	int _rest_time_ms,
	bool _active
):
	spawn_point{_point},
	state{projectile_generator::states::volley},
	velocity{_velocity},
	active{_active},
	type{_type},
	tag{_tag},
	volley_total{_volley}
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

			if(timeouts.is_finished(timeout_volley)) {

				++volley_count;
				timeouts.restart(timeout_volley);

				if(volley_total==volley_count) {

					volley_count=0;
					timeouts.pause(timeout_volley);
					timeouts.restart(timeout_rest);
					state=states::rest;
				}

				return true;
			}
		break;
		case states::rest:

			if(timeouts.is_finished(timeout_rest)) {

				timeouts.pause(timeout_rest);
				timeouts.restart(timeout_volley);
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
	timeouts.restart(timeout_volley);
	timeouts.restart(timeout_rest);
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

projectile_generator::projectile_data projectile_generator::get_projectile_data() const {

	//projectiles should actually generate from the middle
	//of the box, 

	switch(type) {
		default:
		case types::linear:
		case types::directed: //velocity will be used as an absolute value to measure speed.
			return {{velocity, 0.0}, spawn_point};
		case types::vertical: //vertical flame column.
			return {{0.0, velocity}, spawn_point};
		case types::falling: 
			return {{0.0, -fabs(velocity)}, spawn_point};
	}
}
