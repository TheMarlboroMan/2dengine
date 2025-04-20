#include "app/projectile_generator.h"
#include <iostream>

using namespace app;

projectile_generator::projectile_generator(
	d2d::collision::point _point,
	types _type,
	int _velocity,
	int _tag,
	int _volley,
	int _pre_time_ms,
	int _pause_time_ms,
	int _rest_time_ms,
	bool _active
):
	spawn_point{_point},
	state{projectile_generator::states::pre},
	velocity{(float)_velocity},
	active{_active},
	type{_type},
	tag{_tag},
	volley_total{_volley}
{

	double pre_time=(double)_pre_time_ms / 1000.;
	double pause_time=(double)_pause_time_ms / 1000.;
	double rest_time=(double)_rest_time_ms / 1000.;

	//All timeouts start paused.
	timeouts.add(timeout_pre, pre_time, 0., true);
	timeouts.add(timeout_volley, pause_time, 0., true);
	timeouts.add(timeout_rest, rest_time, 0., true);

	if(active) {

		timeouts.restart(timeout_pre);
	}
}

bool projectile_generator::tic(
	ldtools::tdelta _delta
) {

	if(!active) {

		return false;
	}

//This goes [ pre -> [ fire -> volley_pause ] -> post.]

	switch(state) {

		case states::pre:

			timeouts.tic(timeout_pre, _delta);
			if(timeouts.is_finished(timeout_pre)) {

				state=states::fire;
			}

			return false;

		case states::fire:

			++volley_count;
			//Always go into volley pause now...
			timeouts.restart(timeout_volley);
			state=states::volley_pause;

			return true;

		case states::volley_pause:

			timeouts.tic(timeout_volley, _delta);
			if(timeouts.is_finished(timeout_volley)) {

				//Last shot?
				if(volley_count >= volley_total) {

					volley_count=0;
					timeouts.restart(timeout_rest);
					state=states::rest;
				}
				//Do we have shots left?
				else {

					state=states::fire;
				}
			 }

			return false;

		case states::rest:

			timeouts.tic(timeout_rest, _delta);
			if(timeouts.is_finished(timeout_rest)) {

				timeouts.restart(timeout_pre);
				state=states::pre;
			}

			return false;
	}

	return false;
}

void projectile_generator::activate() {

	active=true;
	volley_count=0;
	state=states::pre;
	timeouts.restart(timeout_pre);
}

void projectile_generator::deactivate() {

	active=false;
}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const app::projectile_generator& _generator
) {

	_stream<<"projectile generator[ pos:"<<_generator.spawn_point
		<<" pre:"<<_generator.timeouts.at(projectile_generator::timeout_pre)
		<<" pause:"<<_generator.timeouts.at(projectile_generator::timeout_volley)
		<<" post:"<<_generator.timeouts.at(projectile_generator::timeout_rest)<<"]";

	return _stream;
}

projectile_generator::projectile_data projectile_generator::get_projectile_data() const {

	auto build=[this]() -> projectile_data {

		switch(type) {
			default:
			case types::horizontal:
				return {{velocity, 0.0}, {spawn_point, projectile_horizontal_w, projectile_horizontal_h}, projectile_horizontal_desintegration_ms };

			case types::vertical: //vertical flame column.
				return {{0.0, velocity}, {spawn_point, projectile_vertical_w, projectile_vertical_h}, projectile_vertical_desintegration_ms};

			case types::directed: //velocity will be used as an absolute value to measure speed.
				return {{velocity, 0.0}, {spawn_point, projectile_round_w, projectile_round_h}, projectile_round_desintegration_ms};

			case types::falling: 
				return {{0.0, -fabs(velocity)}, {spawn_point, projectile_falling_w, projectile_falling_h}, projectile_falling_desintegration_ms};
		}
	};

	auto result=build();
	result.box.origin.x-=result.box.w/2;
	result.box.origin.y-=result.box.h/2;

	return result;
}
