#include "app/map_attribute_loader.h"
#include "app/definitions.h"
#include <vector>

using namespace app;

map_attribute_loader::map_attribute_loader(
	ldv::rgba_color& _background_color,
	int& _music_id,
	int& _background_effect,
	bool& _in_game
):
	background_color{_background_color},
	music_id{_music_id},
	background_effect{_background_effect},
	in_game{_in_game}
{}

void map_attribute_loader::setup() {

	background_color={0,0,0, 255};
	music_id=0;
	background_effect=0;
	in_game=true;
}

void map_attribute_loader::load(
	const std::map<std::string, d2d::storage::attribute>& _attr_map
) {

	switch(_attr_map.at("bg_color").get_int()) {

		//sky blue.
		case 1: background_color=ldv::rgba8(49, 162, 242, 255); break;

		//orange
		case 2: background_color=ldv::rgba8(235, 137, 49, 255); break;

		//winter
		case 3: background_color=ldv::rgba8(224, 111, 139, 255); break;

		//starfield, black, and activates a special effect.
		case 4:
			background_color=ldv::rgba8(0, 0, 0, 255);
			background_effect=bg_starfield;
		break;

		//storm, black, and activates a special effect.
		case 5:
			background_color=ldv::rgba8(0, 0, 0, 255);
			background_effect=bg_storm;
		break;
		//suspension, black, and activates a special effect.
		case 6:
			background_color=ldv::rgba8(0, 0, 0, 255);
			background_effect=bg_suspension;
		break;
		//pure white,
		case 7:
			background_color=ldv::rgba8(255, 255, 255, 255);
		break;

		default: //default is black.
		case 0: background_color=ldv::rgba8(0, 0, 0, 255); break;
	}

	music_id=_attr_map.at("music_id").get_int();
	in_game=(bool)_attr_map.at("in_game").get_int();
}
