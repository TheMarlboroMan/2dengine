#include "app/player.h"
#include "app/definitions.h"

using namespace app;

player::player():
	ent{0, 0, app::player_w, app::player_h}
{

	timeouts.add(timeout_ladder, 0.5f, 0.0f);
	timeouts.add(timeout_last_jump_chance, 0.05f, 0.0f);
}

void player::tic(
	float _delta
) {

	timeouts.tic(_delta);
}
