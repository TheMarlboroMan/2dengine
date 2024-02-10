#include "d2d/storage/map_loader.h"
#include "d2d/storage/thing_processor.h"

#include <tools/json.h>
#include <tools/file_utils.h>

#include <stdexcept>
#include <sstream>
#include <iostream>

using namespace d2d::storage;

map_loader::map_loader(
	const std::string& _filename
) {

	try {
		doc=tools::parse_json_string(
			tools::dump_file(_filename)
		);
	}
	catch(std::exception& e) {

		throw std::runtime_error(
			std::string{"error loading map "}
			+_filename
			+" : "
			+e.what()
		);
	}
}

bool map_loader::has_layer(
	const std::string& _layer_type,
	const std::string& _layer_id
) const {

	for(const auto& layer : doc["layers"].GetArray() ) {

		if(!layer.IsObject()) {

			throw std::runtime_error("all layers are expected to be objects");
		}

		if(layer["meta"]["type"].GetString()==_layer_type && layer["meta"]["id"].GetString()==_layer_id) {

			return true;
		}
	}

	return false;
}

const rapidjson::Value& map_loader::locate_layer(
	const std::string& _layer_type,
	const std::string& _layer_id
) const {

	for(const auto& layer : doc["layers"].GetArray() ) {

		if(!layer.IsObject()) {

			throw std::runtime_error("all layers are expected to be objects");
		}

		if(layer["meta"]["type"].GetString()==_layer_type && layer["meta"]["id"].GetString()==_layer_id) {

			return layer;
		}
	}

	std::stringstream ss;
	ss<<"could not locate layer of type "<<_layer_type<<" with id "<<_layer_id;
	throw std::runtime_error(ss.str());
}

void map_loader::load_thing_layer(
	const std::string& _layer_id,
	thing_processor& _processor
) {

	_processor.setup();

	const auto& layer=locate_layer("things", _layer_id);
	for(const auto& thing : layer["data"].GetArray()) {

		auto pos=parse_position(thing);
		int type=thing["t"].GetInt();
		const auto& attr_map=map_attributes(thing["a"]);

		_processor.load(pos, type, attr_map);
	}
}

void map_loader::load_collision_tiles(
	std::vector<d2d::world::collision_tile>& _tiles,
	d2d::collision::shaper& _shaper,
	const d2d::world::collision_tile_implementation& _tileimpl
) {

	_tiles.clear();
	const auto& layer=locate_layer("tiles", "logic");
	for(const auto& tile : layer["data"].GetArray()) {

		auto pos=parse_position(tile);
		int type=tile["t"].GetInt();

		d2d::world::collision_tile tileitem{
			pos.x,
			pos.y,
			type,
			_shaper,
			_tileimpl
		};

		_tiles.push_back(tileitem);
	}
}

/*
			if(layer["meta"]["type"].GetString()==std::string{"tiles"} && layer["meta"]["id"].GetString()==std::string{"background"}) {


				for(const auto& tile : layer["data"].GetArray()) {

					auto pos=parse_position(tile);
					int type=tile["t"].GetInt();

					app::scenery_tile tileitem{
						pos.x,
						pos.y,
						type
					};

					result.raw_bg_tiles.push_back(tileitem);
				}
			}

			if(layer["meta"]["type"].GetString()==std::string{"tiles"} && layer["meta"]["id"].GetString()==std::string{"foreground"}) {

				for(const auto& tile : layer["data"].GetArray()) {

					auto pos=parse_position(tile);
					int type=tile["t"].GetInt();

					app::scenery_tile tileitem{
						pos.x,
						pos.y,
						type
					};

					result.raw_fg_tiles.push_back(tileitem);
				}
			}

			enum thing_types{
				entry=1,
				exit=2,
				ladder=3,
				touch_trigger=4,
				action_trigger=5,
				text_object=6,
				camera_corner=7,
				decoration=8,
				moving_block=9,
				hurt_block=10,
				timer=11,
				counter=12,
				opaque_block=13
			};

			if(layer["meta"]["type"].GetString()==std::string{"things"} && layer["meta"]["id"].GetString()==std::string{"things"}) {

				for(const auto& thing : layer["data"].GetArray()) {

					switch(thing["t"].GetInt()) {
						case entry: parse_entry(thing, result); break;
						case exit:  parse_exit(thing, result); break;
						case ladder: parse_ladder(thing, result); break;
						case touch_trigger: parse_touch_trigger(thing, result); break;
						case action_trigger: parse_action_trigger(thing, result); break;
						case text_object: parse_text_object(thing, result); break;
						case camera_corner: parse_camera_corner(thing, result); break;
						case decoration: parse_decoration(thing, result); break;
						case moving_block: parse_moving_block(thing, result); break;
						case hurt_block: parse_hurt_block(thing, result); break;
						case timer: parse_timer(thing, result); break;
						case counter: parse_counter(thing, result); break;
						case opaque_block: parse_opaque_block(thing, result); break;
					}
				}
			}
*/

position map_loader::parse_position(
	const rapidjson::Value& _node) {

	const auto pos=_node["p"].GetArray();
	return {pos[0].GetInt(), pos[1].GetInt()};
}

std::map<std::string, attribute> map_loader::map_attributes(
	const rapidjson::Value& _attributes
) {

	if(!_attributes.IsObject()) {

		throw std::runtime_error("attributes is expected to be an object!");
	}

	std::map<std::string, attribute> result;
	for(const auto& _node : _attributes.GetObject()) {

		const std::string name=_node.name.GetString();

		if(_node.value.IsInt()) {

			result.emplace(std::make_pair(name, attribute{_node.value.GetInt()}));
			continue;
		}

		if(_node.value.IsString()) {

			result.emplace(std::make_pair(name, attribute{std::string{_node.value.GetString()}}));
			continue;
		}

		if(_node.value.IsBool()) {

			result.emplace(std::make_pair(name, attribute{_node.value.GetBool()}));
			continue;
		}

		throw std::runtime_error("invalid type in attribute, only integers, booleans and strings are supported");

	}

	return result;
}

