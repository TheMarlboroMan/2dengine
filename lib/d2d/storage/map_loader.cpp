#include "d2d/storage/map_loader.h"

#include <tools/json.h>
#include <tools/file_utils.h>

#include <stdexcept>
#include <map>

#include <iostream>
using namespace d2d::storage;

void map_loader::load_from_file_into_map(
	const std::string& _filename,
	d2d::world::map& _map,
	d2d::collision::shaper& _shaper
) {
	try {
		const auto doc=tools::parse_json_string(
			tools::dump_file(_filename)
		);

/*
		result.name=doc["attributes"]["name"].GetString();
		result.music_id=doc["attributes"]["music_id"].GetInt();
*/


		for(const auto& layer : doc["layers"].GetArray() ) {

			if(layer["meta"]["type"].GetString()==std::string{"tiles"} && layer["meta"]["id"].GetString()==std::string{"logic"}) {

				auto& collision_tiles=_map.get_collision_tiles();
				for(const auto& tile : layer["data"].GetArray()) {

					auto pos=parse_position(tile);
					int type=tile["t"].GetInt();

					d2d::world::collision_tile tileitem{
						pos.x,
						pos.y,
						type,
						_shaper
					};

					collision_tiles.push_back(tileitem);
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
		};
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

map_loader::position map_loader::parse_position(
	const rapidjson::Value& _node
) {

	const auto pos=_node["p"].GetArray();
	return {pos[0].GetInt(), pos[1].GetInt()};
}

