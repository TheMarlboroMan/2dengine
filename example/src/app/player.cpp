#include "app/player.h"
#include "app/definitions.h"
#include "app/debug.h"

using namespace app;

player::player():
	ent{0, 0, player_w, player_h}
{

	timeouts.add(timeout_ladder, 0.3);
	timeouts.add(timeout_last_jump_chance, 0.1);
	timeouts.add(timeout_defeat, 2.);
	timeouts.add(timeout_jump_buffer, 0.1, 0.1);
}

void player::tic(
	ldtools::tdelta _delta
) {

/**
#ifdef IS_DEBUG_BUILD

	std::stringstream ss;
	ss<<"ladder "<<timeouts.at(timeout_ladder)
		<<" jump "<<timeouts.at(timeout_last_jump_chance)
		<<" buffer "<<timeouts.at(timeout_jump_buffer)
		<<" defeat "<<timeouts.at(timeout_defeat)
		<<"\n";
	app::debug::log().debug()<<ss.str();

#endif
*/

	timeouts.tic(_delta);
}

void player::crouch() {

	ent.set_motion_vector_x(0.);
	state=states::crouch;
	ent.get_box().h=player_h_crouch;
}

void player::stand_up() {

	ent.set_motion_vector_x(0.);
	state=states::ground;
	ent.get_box().h=player_h;
}

void player::jump(
	double _force
) {

	ent.set_motion_vector_y(_force);
	state=states::air;
	jump_shortened=false;
}

void player::buffer_jump() {

	timeouts.restart(player::timeout_jump_buffer);
}

bool player::is_defeated() const {

	return state==states::defeat;
}

bool player::is_crouched() const {

	return state==states::crouch;
}

void player::reset() {

	facing=faces::right;
	timeouts.restart();
	current_ladder=nullptr;
	jump_shortened=false;
}

/**
* launch means "be launched by a push trigger".
**/
void player::launch(
	const d2d::motion::motion_vector _vec
) {

	if(0.0 != _vec.y) {

		ent.set_motion_vector_y(_vec.y);
		state=states::air;
		jump_shortened=true; //The player cannot influence this.
	}

	if(0.0 != _vec.x) {

		ent.set_motion_vector_x(_vec.x);
		state=states::air;
	}
}

void player::defeat(
	double _velocity
) {

	timeouts.restart(timeout_defeat);
	state=player::states::defeat;
	ent.set_motion_vector_y(_velocity);
}

void player::walk_out_of_ladder(
	int _x_force
) {

	state=states::ground;
	timeouts.restart(app::player::timeout_ladder);

	facing=_x_force > 0 
		? app::faces::right
		: app::faces::left;

	current_ladder=nullptr;
}

void player::jump_out_of_ladder(
	double _velocity,
	double _jump_force
) {

	ent.set_motion_vector_x(_velocity);

	facing=_velocity > 0 
		? app::faces::right
		: app::faces::left;
	current_ladder=nullptr;
	timeouts.target(app::player::timeout_ladder, 0.3)
		.restart();

	jump(_jump_force);
}

void player::drop_out_of_ladder() {

	ent.set_motion_vector_x(0.);
	current_ladder=nullptr;
	state=player::states::air;

	timeouts.target(app::player::timeout_ladder, 0.5)
		.restart();
	//there is no last chance jump here.
}

void player::start_falling() {

	state=player::states::air;
	timeouts.restart(player::timeout_last_jump_chance);

	auto velocity=ent.get_motion_vector();
	ent.set_motion_vector_x(velocity.x/2.);
}

bool player::has_jump_last_chance() const {

	return timeouts.is_running(timeout_last_jump_chance);
}

bool player::has_jump_buffered() const {

	return !timeouts.is_finished(timeout_jump_buffer);
}

bool player::is_defeat_timeout_done() const {

	return !timeouts.is_running(timeout_defeat);
}

bool player::is_ladder_timeout_done() const {

	return timeouts.is_finished(timeout_ladder);
}

/*
d2d::collision::box player::get_standing_box() const {

	auto box=ent.get_box();
	box.h=player_h;
	return box;
}
*/
