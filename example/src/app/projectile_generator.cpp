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

	float pre_time=(float)_pre_time_ms / 1000.f;
	float pause_time=(float)_pause_time_ms / 1000.f;
	float rest_time=(float)_rest_time_ms / 1000.f;

	//All timeouts start paused.
	timeouts.add(timeout_pre, pre_time, -1.f, true);
	timeouts.add(timeout_volley, pause_time, -1.f, true);
	timeouts.add(timeout_rest, rest_time, -1.f, true);

	if(active) {

		timeouts.restart(timeout_pre);
	}
}

bool projectile_generator::tic(
	float _delta
) {

	if(!active) {

		return false;
	}

	timeouts.tic(_delta);

//This goes [ pre -> [ fire -> volley_pause ] -> post.]

	switch(state) {

		case states::pre:

std::cout<<spawn_point<<" PRE "<<timeouts.at(timeout_pre)<<std::endl;

			if(timeouts.is_finished(timeout_pre)) {

				state=states::fire;
			}
		break;

		case states::fire:

			++volley_count;
			//Always go into volley pause now...
			timeouts.restart(timeout_volley);
			state=states::volley_pause;

			return true;

		case states::volley_pause:

std::cout<<spawn_point<<" PAUSE "<<timeouts.at(timeout_volley)<<std::endl;

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

		break;
		case states::rest:

std::cout<<spawn_point<<" REST "<<timeouts.at(timeout_rest)<<std::endl;

			if(timeouts.is_finished(timeout_rest)) {

				state=states::pre;
				timeouts.restart(timeout_pre);
			}
		break;
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
	const app::projectile_generator&
) {

	_stream<<"projectile generator[]";
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
