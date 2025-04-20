#include "app/player.h"
#include "app/definitions.h"
#include "app/debug.h"

using namespace app;

player::player():
	ent{0, 0, player_w, player_h}
{

	timeouts.add(timeout_ladder, 0.5);
	timeouts.add(timeout_last_jump_chance, 0.05);
	timeouts.add(timeout_defeat, 2.);
}

void player::tic(
	ldtools::tdelta _delta
) {

/**
#ifdef IS_DEBUG_BUILD

	std::stringstream ss;
	ss<<"ladder "<<timeouts.at(timeout_ladder)
		<<" jump "<<timeouts.at(timeout_last_jump_chance)
		<<" defeat "<<timeouts.at(timeout_defeat)
		<<"\n";
	app::debug::log().debug()<<ss.str();

#endif
*/

	timeouts.tic(_delta);
}

void player::crouch() {

	velocity.x=0.;
	state=states::crouch;
	ent.get_box().h=player_h_crouch;
}

void player::stand_up() {

	velocity.x=0.;
	state=states::ground;
	ent.get_box().h=player_h;
}

void player::jump(
	double _force
) {

	velocity.y=_force;
	state=states::air;
	jump_shortened=false;
}

bool player::is_defeated() const {

	return state==states::defeat;
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

		velocity.y=_vec.y;
		state=states::air;
		jump_shortened=true; //The player cannot influence this.
	}

	if(0.0 != _vec.x) {

		velocity.x=_vec.x;
		state=states::air;
	}
}

void player::defeat(
	double _velocity
) {

	timeouts.restart(timeout_defeat);
	state=player::states::defeat;
	velocity.y=_velocity;
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

	velocity.x=_velocity;

	facing=_velocity > 0 
		? app::faces::right
		: app::faces::left;
	current_ladder=nullptr;
	timeouts.restart(app::player::timeout_ladder);

	jump(_jump_force);
}

void player::drop_out_of_ladder() {

	velocity.x=0.;
	current_ladder=nullptr;
	state=player::states::air;
	timeouts.restart(player::timeout_ladder);
	//there is no last chance jump here.
}

void player::start_falling() {

	state=player::states::air;
	timeouts.restart(player::timeout_last_jump_chance);
	velocity.x/=2.; 
}

bool player::has_jump_last_chance() const {

	return timeouts.is_running(timeout_last_jump_chance);
}

bool player::is_defeat_timeout_done() const {

	return !timeouts.is_running(timeout_defeat);
}

bool player::is_ladder_timeout_done() const {

	return timeouts.is_finished(timeout_ladder);
}
