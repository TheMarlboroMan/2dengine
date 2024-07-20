#include "app/game_session.h"
#include "app/definitions.h"
#include <stdexcept>

using namespace app;

void game_session::reset(
	int _value
) {

	set_skill_level(_value);
	lives=0;
	seconds_elapsed=0;
	current_map_id=0;
}

void game_session::set_skill_level(
	int _value
) {

	switch(_value) {

		case app::skill_easy:
		case app::skill_normal:
		case app::skill_hard:

			skill_level=_value;
			return;
	}

	throw std::runtime_error("bad difficulty setting");
}

bool game_session::with_lives() const {

	return skill_level & app::skill_hard;
}

bool game_session::with_timer() const {

	return skill_level != app::skill_easy;
}
