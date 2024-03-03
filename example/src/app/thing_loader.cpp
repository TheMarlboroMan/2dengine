#include "app/thing_loader.h"
#include "app/definitions.h"
#include <iostream>
#include <sstream>

using namespace app;

thing_loader::thing_loader(
	map& _map,
	tpersistence& _persistence
):
	curmap{_map},
	persistence{_persistence}
{}

void thing_loader::setup() {

	curmap.solid_blocks.clear();
	curmap.platform_blocks.clear();
	curmap.ladders.clear();
	curmap.entries.clear();
}

void thing_loader::load(
	d2d::storage::position _pos,
	int _type, 
	const std::map<std::string, d2d::storage::attribute>& _attributes
) {

	try {

		switch(_type) {

			case 1:{

				unsigned w=_attributes.at("width").get_int();
				unsigned h=_attributes.at("height").get_int();
				int id=_attributes.at("entry_id").get_int();
				int type=_attributes.at("position").get_int();
				curmap.entries.push_back({ { {_pos.x, _pos.y}, w, h}, id, type});
				return;
			}
			case 2: {

				unsigned w=_attributes.at("width").get_int();
				unsigned h=_attributes.at("height").get_int();
				int next_id=_attributes.at("next_entry_id").get_int();
				bool touch=_attributes.at("type").get_int()==1;
				std::string filename=_attributes.at("map_filename").get_string();

				curmap.exits.push_back({
					{ {_pos.x, _pos.y}, w, h},
					filename,
					next_id,
					touch
				});
				return;
			}
			case 4:

				curmap.ladders.push_back(
					{_pos.x, _pos.y, _attributes.at("height").get_int()*app::tile_h}
				);
				return;
			case 5:{

				int id=_attributes.at("id").get_int();
				//Check if it was already retrieved.
				if(persistence.has(pergr_collectibles, id)) {

					return;
				}

				int type=_attributes.at("type").get_int();
				curmap.collectibles.push_back(
					{ {_pos.x, _pos.y}, id, type}
				);
				return;
			}

			case 50: {

				int type=_attributes.at("type").get_int();
				bool faces_right=_attributes.at("facing_right").get_int()==1;
				curmap.linear_monsters.push_back(
					{ {_pos.x, _pos.y}, type, faces_right}
				);

				return;
			}
			case 100:

				curmap.platform_blocks.push_back(
					{_pos.x, _pos.y, _attributes.at("width").get_int()}
				);
				return;
			case 101:

				curmap.solid_blocks.push_back(
					{_pos.x, _pos.y, _attributes.at("width").get_int(), _attributes.at("height").get_int()}
				);
				return;
		}
	}
	catch(std::exception& e) {

		std::ostringstream ss;
		ss<<"thing loader failed for pos:"<<_pos.x<<","<<_pos.y<<", type:"<<_type<<" err: "<<e.what();
		throw std::runtime_error(ss.str());
	}
}
