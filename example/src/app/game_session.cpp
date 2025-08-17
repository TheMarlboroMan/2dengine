#include "app/game_session.h"
#include "app/definitions.h"
#include <stdexcept>

using namespace app;

void game_session::reset(
	int _value,
	const std::string& _savegame_file
) {

	set_skill_level(_value);
	savegame_file=_savegame_file;
	lives=0;
	elapsed_seconds=0;
	current_map_id=0;
	discovered_rooms=0;
	game_clock.reset();
}

void game_session::set_skill_level(
	int _value
) {

	switch(_value) {

		case app::skill_easy:
			skill_level=_value;
			return;

		case app::skill_normal:
			skill_level=_value;
			return;

		case app::skill_hard:
			lives=3;
			skill_level=_value;
			return;
	}

	

	throw std::runtime_error("bad difficulty setting");
}

bool game_session::is_with_lives() const {

	return skill_level & app::skill_hard;
}

bool game_session::is_with_timer() const {

	return skill_level != app::skill_easy;
}

int game_session::get_discovered_map_count() const {

#ifdef IS_DEBUG_BUILD

	//Allow for faking this in debug mode, so we can test stuff out..
	if(-1!=debug_discovered_rooms) {

		return debug_discovered_rooms;
	}

#endif

	return discovered_rooms;
}
