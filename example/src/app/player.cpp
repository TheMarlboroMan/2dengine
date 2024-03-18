#include "app/player.h"
#include "app/definitions.h"

using namespace app;

player::player():
	ent{0, 0, player_w, player_h}
{

	timeouts.add(timeout_ladder, 0.5f);
	timeouts.add(timeout_last_jump_chance, 0.05f);
	timeouts.add(timeout_defeat, 2.f);
}

void player::tic(
	float _delta
) {

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
