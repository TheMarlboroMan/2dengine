#include "app/thing_loader.h"
#include <iostream>

using namespace app;

thing_loader::thing_loader(
	map& _map
)
	:starting_position{0,0},
	curmap{_map}
{}

void thing_loader::setup() {

	curmap.solid_blocks.clear();
	curmap.platform_blocks.clear();
}

void thing_loader::load(
	d2d::storage::position _pos,
	int _type, 
	const std::map<std::string, d2d::storage::attribute>& _attributes
) {

	switch(_type) {

		case 1:
			starting_position={_pos.x, _pos.y};
			return;
		case 2:
			curmap.platform_blocks.push_back(
				{_pos.x, _pos.y, _attributes.at("width").get_int()}
			);
			return;
		case 3:
			curmap.solid_blocks.push_back(
				{_pos.x, _pos.y, _attributes.at("width").get_int(), _attributes.at("height").get_int()}
			);
			return;
	}
}
