#include "app/thing_loader.h"
#include <iostream>

using namespace app;

thing_loader::thing_loader(
	std::vector<solid_block>& _solid_blocks
)
	:starting_position{0,0},
	solid_blocks{_solid_blocks}
{}

void thing_loader::setup() {

	solid_blocks.clear();
}

void thing_loader::load(
	d2d::storage::position _pos,
	int _type, 
	const std::map<std::string, d2d::storage::attribute>& _attributes
) {

	switch(_type) {

		case 1:
			starting_position={_pos.x, _pos.y};
		break;
		case 3:
			solid_blocks.push_back(
				{_pos.x, _pos.y, _attributes.at("width").get_int(), _attributes.at("height").get_int()}
			);
		break;
	}
}
