#include "app/thing_loader.h"
#include "app/definitions.h"
#include "d2d/collision/definitions.h"
#include "d2d/collision/tile_finder_matrix.h"
#include <iostream>
#include <sstream>

using namespace app;

thing_loader::thing_loader(
	map& _map,
	d2d::collision::tile_limits _limits,
	tpersistence& _persistence,
	int _difficulty_setting
):
	curmap{_map},
	tile_limits{_limits},
	persistence{_persistence},
	difficulty_setting{_difficulty_setting}
{}

void thing_loader::setup() {

	curmap.platform_blocks.clear();
	curmap.ladders.clear();
	curmap.entries.clear();
	curmap.collectibles.clear();
	curmap.secret_covers.clear();
	curmap.buttons.clear();
	curmap.touch_triggers.clear();
	curmap.gates.clear();
	curmap.projectile_generators.clear();
	curmap.linear_monsters.clear();
	curmap.leaping_monsters.clear();
	curmap.breaking_platforms.clear();
	curmap.timed_traps.clear();
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
			case 6: return add_secret_cover(pos, _attributes);
			case 7: return add_button(pos, _attributes);
			case 8: return add_gate(pos, _attributes);
			case 9: return add_breaking_platform(pos, _attributes);
			case 10: return add_platform(pos, _attributes);
			case 11: return add_touch_trigger(pos, _attributes);

			case 50: return add_linear_monster(pos, _attributes);
			case 51: return add_projectile_generator(pos, _attributes);
			case 52: return add_leaping_monster(pos, _attributes);
			case 53: return add_timed_trap(pos, _attributes);

			//Adding something here? clear it up in "setup!".
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

	int w=_attributes.at("width").get_int();
	int h=_attributes.at("height").get_int();
	int id=_attributes.at("entry_id").get_int();
	int type=_attributes.at("position").get_int();
	curmap.entries.push_back({ { {_pos.x, _pos.y}, w, h}, id, type});
}

void thing_loader::add_exit(
	d2d::collision::point _pos,
	const thing_loader::attrmap& _attributes
) {

	int w=_attributes.at("width").get_int();
	int h=_attributes.at("height").get_int();
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

	int difficulty_flags=_attributes.at("difficulty").get_int();
	if(!(difficulty_flags & difficulty_setting)) {

		return;
	}

	int height=_attributes.at("height").get_int()*app::tile_h;
	int type=_attributes.at("type").get_int();

	curmap.ladders.push_back(
		{_pos, height, type} 
	);
}

void thing_loader::add_collectible(
	d2d::collision::point _pos,
	const thing_loader::attrmap& _attributes
) {

	int difficulty_flags=_attributes.at("difficulty").get_int();
	if(!(difficulty_flags & difficulty_setting)) {

		return;
	}

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

void thing_loader::add_secret_cover(
	d2d::collision::point _pos,
	const thing_loader::attrmap& _attributes
) {

	int id=_attributes.at("id").get_int();

	//Check if it was already discovered.
	if(persistence.has(pergr_secret_covers, id)) {

		return;
	}

	int w=_attributes.at("width").get_int()*app::tile_w;
	int h=_attributes.at("height").get_int()*app::tile_h;

	curmap.secret_covers.push_back(
		{ {{_pos.x, _pos.y}, w, h}, id}
	);
}

void thing_loader::add_linear_monster(
	d2d::collision::point _pos,
	const thing_loader::attrmap& _attributes
) {

	int difficulty_flags=_attributes.at("difficulty").get_int();
	if(!(difficulty_flags & difficulty_setting)) {

		return;
	}

	app::linear_monster::types type{app::linear_monster::types::scorpion};
	double lower_bound=0.;
	double upper_bound=0.;

	auto tile=curmap.tile_finder.to_tile_coords(_pos);

	switch(_attributes.at("type").get_int()) {

		case 1: 
			type=app::linear_monster::types::scorpion; 
			lower_bound=find_lower_x_bound(tile);
			upper_bound=find_upper_x_bound(tile);
		break;
		case 2: 
			type=app::linear_monster::types::snake; 
			lower_bound=find_lower_x_bound(tile);
			upper_bound=find_upper_x_bound(tile);
		break;
		case 3: 
			type=app::linear_monster::types::bat; 
			lower_bound=find_lower_y_bound(tile);
			upper_bound=find_upper_y_bound(tile);
		break;
	}

	bool faces_right=_attributes.at("facing_right").get_int()==1;

	curmap.linear_monsters.push_back(
		{ {_pos.x, _pos.y}, type, faces_right, {lower_bound, upper_bound}}
	);
}

void thing_loader::add_leaping_monster(
	d2d::collision::point _pos,
	const thing_loader::attrmap& _attributes
) {

	int difficulty_flags=_attributes.at("difficulty").get_int();
	if(!(difficulty_flags & difficulty_setting)) {

		return;
	}

	int rest_ms=_attributes.at("rest_ms").get_int();
	int leap_force=_attributes.at("leap_force").get_int();

	curmap.leaping_monsters.push_back(
		{ {_pos.x, _pos.y}, app::leaping_monster::types::piranha, rest_ms, leap_force}
	);
}

void thing_loader::add_timed_trap(
	d2d::collision::point _pos,
	const thing_loader::attrmap& _attributes
) {

	int difficulty_flags=_attributes.at("difficulty").get_int();
	if(!(difficulty_flags & difficulty_setting)) {

		return;
	}

	bool active=_attributes.at("active").get_int()==1;
	int tag=_attributes.at("tag").get_int();
	int pre_ms=_attributes.at("pre_ms").get_int();
	int active_ms=_attributes.at("active_ms").get_int();
	int post_ms=_attributes.at("post_ms").get_int();

	//Spawn it aligned to the center of the grid.
	_pos.x+=(app::tile_w / 2)- (app::timed_trap::fire_w / 2);

	curmap.timed_traps.push_back(
		{ {_pos.x, _pos.y}, app::timed_trap::types::fire, active, tag, pre_ms, active_ms, post_ms}
	);
}

void thing_loader::add_button(
	d2d::collision::point _pos,
	const thing_loader::attrmap& _attributes
) {

	int id=_attributes.at("id").get_int();
	bool used=persistence.has(pergr_buttons, id);
	int tag=_attributes.at("tag").get_int();

	button::types type=button::types::regular;
	switch(_attributes.at("type").get_int()) {

		case 1: type=button::types::regular; break;
		case 2: type=button::types::yellow_keyhole; break;
		case 3: type=button::types::blue_keyhole; break;
		case 4: type=button::types::red_keyhole; break;
		case 5: type=button::types::green_keyhole; break;
	}

	curmap.buttons.push_back(
		{ _pos, type, id, tag, used}
	);
}

void thing_loader::add_gate(
	d2d::collision::point _pos, 
	const thing_loader::attrmap& _attributes
) {

	int height=_attributes.at("height").get_int()*app::tile_h;
	int tag=_attributes.at("tag").get_int();

	curmap.gates.push_back(
		{
			{_pos, app::tile_w, height}, 
			tag
		} 
	);
}

void thing_loader::add_projectile_generator(
	d2d::collision::point _pos, 
	const thing_loader::attrmap& _attributes
) {

	app::projectile_generator::types type{app::projectile_generator::types::horizontal};
	switch(_attributes.at("type").get_int()) {

		case 0:
			type=app::projectile_generator::types::horizontal;
		break;
		case 1:
			type=app::projectile_generator::types::directed;
		break;
		case 2:
			type=app::projectile_generator::types::falling;
		break;
		case 3:
			type=app::projectile_generator::types::vertical;
		break;
	}

	int velocity=_attributes.at("velocity").get_int();
	int tag=_attributes.at("tag").get_int();
	int volley_count=_attributes.at("volley_count").get_int();
	int pre_ms=_attributes.at("pre_ms").get_int();
	int pause_ms=_attributes.at("pause_ms").get_int();
	int rest_ms=_attributes.at("rest_ms").get_int();
	bool active=_attributes.at("active").get_int()==1;

	//position is adjusted to be exactly in the
	//middle of a tile:
	_pos.x+=app::tile_w/2;
	_pos.y+=app::tile_h/2;

	curmap.projectile_generators.push_back(
		{
			_pos, 
			type,
			velocity,
			tag,
			volley_count,
			pre_ms,
			pause_ms,
			rest_ms,
			active
		} 
	);
}

void thing_loader::add_breaking_platform(
	d2d::collision::point _pos, 
	const thing_loader::attrmap& _attributes
) {

	int ms_breaking=_attributes.at("ms_breaking").get_int();
	int ms_gone=_attributes.at("ms_gone").get_int();
	int ms_returning=_attributes.at("ms_returning").get_int();

	curmap.breaking_platforms.push_back(
		{
			_pos,
			ms_breaking,
			ms_gone,
			ms_returning
		} 
	);
}

void thing_loader::add_platform(
	d2d::collision::point _pos, 
	const thing_loader::attrmap& _attributes
) {

	int difficulty_flags=_attributes.at("difficulty").get_int();
	if(!(difficulty_flags & difficulty_setting)) {

		return;
	}

	app::platform_block::types type{app::platform_block::types::branch};

	switch(_attributes.at("type").get_int()) {

		case 0: type=app::platform_block::types::branch; break;
		case 1: type=app::platform_block::types::rock; break;
		default: break;
	}

	curmap.platform_blocks.push_back({_pos, type });
}

double thing_loader::find_lower_x_bound(
	d2d::collision::tile_coords _tile
) const {

	//the limits start as wide as they can, then we narrow them.
	double lower_bound=tile_limits.left;

	//lower bound.
	for(int x=_tile.x-1; x >= tile_limits.left; x--) {

		if(curmap.tile_finder.has({x, _tile.y})) {

			//notice we pick the next free tile. its to_point will give us 
			//the right blocked tile's edge.
			lower_bound=curmap.tile_finder.to_point({x+1, _tile.y}).x;
			break;
		}
	}

	return lower_bound;
}

double thing_loader::find_upper_x_bound(
	d2d::collision::tile_coords _tile
) const {

	double upper_bound=tile_limits.right;
	for(int x=_tile.x+1; x <= tile_limits.right; x++) {

		if(curmap.tile_finder.has({x, _tile.y})) {

			upper_bound=curmap.tile_finder.to_point({x, _tile.y}).x-app::tile_w;
			break;
		}
	}

	return upper_bound;
}

double thing_loader::find_lower_y_bound(
	d2d::collision::tile_coords _tile
) const {

	double lower_bound=tile_limits.bottom;
	for(int y=_tile.y-1; y >= tile_limits.bottom; y--) {

		if(curmap.tile_finder.has({_tile.x, y})) {

			lower_bound=curmap.tile_finder.to_point({_tile.x, y+1}).y;
			break;
		}
	}

	return lower_bound;
}

double thing_loader::find_upper_y_bound(
	d2d::collision::tile_coords _tile
) const {

	double upper_bound=tile_limits.top;
	for(int y=_tile.y+1; y <= tile_limits.top; y++) {

		if(curmap.tile_finder.has({_tile.x, y})) {

			upper_bound=curmap.tile_finder.to_point({_tile.x, y}).y;
			break;
		}
	}

	return upper_bound;
}

void thing_loader::add_touch_trigger(
	d2d::collision::point _pos,
	const thing_loader::attrmap& _attributes
) {

	int id=_attributes.at("id").get_int();
	int tag=_attributes.at("tag").get_int();
	int width=_attributes.at("width").get_int();
	int height=_attributes.at("height").get_int();
	bool used=persistence.has(pergr_touch_triggers, id);

	curmap.touch_triggers.push_back(
		{ {_pos, width, height}, id, tag, used}
	);
}

