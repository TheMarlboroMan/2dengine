#include "app/thing_loader.h"
#include "app/definitions.h"
#include "d2d/collision/definitions.h"
#include <iostream>
#include <sstream>

using namespace app;

thing_loader::thing_loader(
	map& _map,
	d2d::collision::tile_limits _limits,
	tpersistence& _persistence
):
	curmap{_map},
	tile_limits{_limits},
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
	const thing_loader::attrmap& _attributes
) {

	d2d::collision::point pos{(double)_pos.x, (double)_pos.y};

	try {

		switch(_type) {

			case 1: return add_entry(pos, _attributes);
			case 2: return add_exit(pos, _attributes);
			case 4: return add_ladder(pos, _attributes);
			case 5: return add_collectible(pos, _attributes);
			case 50: return add_linear_monster(pos, _attributes);

			//TODO: These will likely dissapear.
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

void thing_loader::add_entry(
	d2d::collision::point _pos, 
	const thing_loader::attrmap& _attributes
) {

	unsigned w=_attributes.at("width").get_int();
	unsigned h=_attributes.at("height").get_int();
	int id=_attributes.at("entry_id").get_int();
	int type=_attributes.at("position").get_int();
	curmap.entries.push_back({ { {_pos.x, _pos.y}, w, h}, id, type});
}

void thing_loader::add_exit(
	d2d::collision::point _pos,
	const thing_loader::attrmap& _attributes
) {

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
}

void thing_loader::add_ladder(
	d2d::collision::point _pos, 
	const thing_loader::attrmap& _attributes
) {

	curmap.ladders.push_back(
		{_pos.x, _pos.y, _attributes.at("height").get_int()*app::tile_h}
	);
}

void thing_loader::add_collectible(
	d2d::collision::point _pos,
	const thing_loader::attrmap& _attributes
) {

	int id=_attributes.at("id").get_int();

	//Check if it was already retrieved.
	if(persistence.has(pergr_collectibles, id)) {

		return;
	}

	int type=_attributes.at("type").get_int();
	curmap.collectibles.push_back(
		{ {_pos.x, _pos.y}, id, type}
	);
}


void thing_loader::add_linear_monster(
	d2d::collision::point _pos,
	const thing_loader::attrmap& _attributes
) {

	int type=_attributes.at("type").get_int();
	bool faces_right=_attributes.at("facing_right").get_int()==1;


	//TODO: Calculate upper and lower bounds... first we need to get the 
	//tile this is on. 
	//TODO: We have no tool for this. And it would not be remiss if we had
	//one. We can use the tile_finder, right?
	//
	//Next we use the limits and our knowledge of the map to search, we can
	//use a tile_finder for it

	curmap.linear_monsters.push_back(
		{ {_pos.x, _pos.y}, type, faces_right}
	);
}
	
