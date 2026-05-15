#include "app/map_attribute_loader.h"
#include <vector>

using namespace app;

map_attribute_loader::map_attribute_loader(
	ldv::rgba_color& _background_color,
	int& _music_id,
	int& _background_effect
):
	background_color{_background_color},
	music_id{_music_id},
	background_effect{_background_effect}
{}

void map_attribute_loader::setup() {

	background_color={0,0,0, 255};
	music_id=0;
	background_effect=0;
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

		//somewhere else, black, and activates a special effect.
		case 4:
			background_color=ldv::rgba8(0, 0, 0, 255);
			background_effect=1;
		break;

		default: //default is black.
		case 0: background_color=ldv::rgba8(0, 0, 0, 255); break;
	}

	music_id=_attr_map.at("music_id").get_int();

/*
	struct pair {
		const std::string name;
		int     color{0};
	};
	

	std::vector<pair> colors={{"bg_red", 0}, {"bg_green", 0}, {"bg_blue", 0}};

	for(auto& color: colors) {

		if(_attr_map.count(color.name)) {

			color.color=_attr_map.at(color.name).get_int();
		}
	}

	background_color=ldv::rgba8(
		colors[0].color,
		colors[1].color,
		colors[2].color,
		255
	);
*/
}
