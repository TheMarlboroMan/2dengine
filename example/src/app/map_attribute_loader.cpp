#include "app/map_attribute_loader.h"
#include <vector>

using namespace app;

map_attribute_loader::map_attribute_loader(
	ldv::rgba_color& _background_color,
	int& _music_id,
	bool& _save_point
):
	background_color{_background_color},
	music_id{_music_id},
	save_point{_save_point}
{}

void map_attribute_loader::setup() {

	background_color={0,0,0, 255};
	music_id=0;
	save_point=false;
}

void map_attribute_loader::load(
	const std::map<std::string, d2d::storage::attribute>& _attr_map
) {

	switch(_attr_map.at("bg_color").get_int()) {

		//sky blue.
		case 1: background_color=ldv::rgba8(49, 162, 242, 255); break;

		default: //default is black.
		case 0: background_color=ldv::rgba8(0, 0, 0, 255); break;
	}

	music_id=_attr_map.at("music_id").get_int();
	save_point=(bool)_attr_map.at("save_point").get_int();

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
