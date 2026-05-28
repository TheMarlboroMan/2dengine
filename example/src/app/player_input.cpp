#include <app/player_input.h>

using namespace app;

void player_input::reset() {

	x=0;
	y=0;
	jump=false;
	hold_jump=false;
	activate=false;
}

player_input::operator bool() const {

	return x || y || jump || hold_jump || activate;
}
