#include "controller/controller_states.h"
#include "controller/pause.h"
#include "app/input.h"
#include "app/inventory.h"

#include <ldtools/ttf_manager.h>
#include <tools/json.h>
#include <tools/file_utils.h>
#include <tools/time.h>
#include <ldv/box_representation.h>

#include <vector>

using namespace controller;

pause::pause(
	app::service_provider& _sp
):
	env{_sp.get_env()},
	logger{_sp.get_logger()},
	inventory{_sp.get_inventory()},
	localization{_sp.get_localization()},
	persistence{_sp.get_persistence()},
	game_session{_sp.get_game_session()},
	automap{_sp.get_automap()},
	automap_interface{automap},
	map_representation{{0,0}},
	//TODO: Maybe 3 colors: current, complete and incomplete???
	//
	wall_complete{ldv::rgba8(255, 255, 255, 255)},
	wall_incomplete{ldv::rgba8(157, 157, 157, 255)},
	regular_fill{ldv::rgba8(164, 100, 34, 255)},
	current_fill{ldv::rgba8(190, 38, 51, 255)},
	timeout{0.5, 0., false}
{

	view.map_font(
		"pause_font",
		_sp.get_ttf_manager().get("pause_font", 8)
	);

	view.map_texture("main", _sp.get_video_resource_manager().get_texture(app::tex_tiles));

	const std::string layout_path=env.build_app_path("resources/layout/views.json");
	auto document=tools::parse_json_string(tools::dump_file(layout_path));
	view.parse(document["pause"]);
}

void pause::awake(
	dfw::input& /*input*/
) {

	timeout.restart();

	//Hide or show stuff according to game mode.
	view.set_visible("lives_icon", game_session.is_with_lives());
	view.set_visible("lives_value", game_session.is_with_lives());
	view.set_visible("time_value", game_session.is_with_timer());

	view.set_visible("red_key_icon", inventory.red_keys!=0);
	view.set_visible("blue_key_icon", inventory.blue_keys!=0);
	view.set_visible("green_key_icon", inventory.green_keys!=0);
	view.set_visible("ultimate_icon", inventory.ultimate!=0);

	//Update the values to be shown...
	std::stringstream ss;
	ss<<" x "<<inventory.treasure;
	view.set_text("treasure_value", ss.str());

	ss.str("");
	ss<<" x "<<inventory.yellow_keys;
	view.set_text("keys_value", ss.str());

	ss.str("");
	ss<<" x "<<game_session.get_discovered_map_count();
	view.set_text("rooms_value", ss.str());

	if(game_session.is_with_lives()) {

		ss.str("");
		ss<<" x "<<game_session.lives;
		view.set_text("lives_value", ss.str());
	}

	if(game_session.is_with_timer()) {

		ss.str("");
		tools::time t;
		auto seconds_elapsed=game_session.elapsed_seconds+game_session.game_clock.get_seconds();
		auto td=t.seconds_to_timedata(seconds_elapsed);
		ss<<t.time_to_string(td.hours, td.minutes, td.seconds);
		view.set_text("time_value", ss.str());
	}

	//TODO: Actually, we should do this "in game" right??
	//Update the automap so that we can go back and forth between discovered
	//areas...
	for(auto i=automap_interface.get_min_area_id(); i<=automap_interface.get_max_area_id(); i++) {

		//guard against capricious data.
		if(!automap_interface.has(i)) {

			continue;
		}

		automap_interface.set(i);
		for(const auto& cell : automap_interface.get().cells) {

			if(persistence.has(app::pergr_automap, cell.id)) {

				automap_interface.discover_area(i);
				break;
			}
		}
	}
	
	//Finally set the map.
	lm::log(logger).debug()<<"attempting to locate area for "<<game_session.current_map_id<<"\n";

	auto area_id=automap.find_area_by_map_id(game_session.current_map_id).id;
	automap_interface.set(area_id);

	//Disallow map changing when in "nowhere" zones.
	can_change_area=automap_interface.get().must_draw_map;
	ready_view();
}

void pause::slumber(
	dfw::input& /*input*/
) {

}

bool pause::can_leave_state() const {

	return timeout.is_finished();
}

void pause::loop(
	dfw::input& _input,
	const dfw::loop_iteration_data& _lid
) {

	timeout.tic(_lid.delta);

	if(_input().is_exit_signal()) {
		set_leave(true);
		return;
	}

	if(
		_input.is_input_down(app::input::escape)
		|| _input.is_input_down(app::input::pause)
	) {

		pop_state();
		return;
	}

#ifdef IS_DEBUG_BUILD

	if(_input.is_input_down(app::input::tic)) {

		lm::log(logger).debug()<<"toggling 'display_all_maps'\n";
		display_all_maps=!display_all_maps;
		automap_interface.debug_all_discovered=display_all_maps;
		ready_view();
	}
#endif

	if(can_change_area && _input.is_input_down(app::input::left)) {

		automap_interface.previous();
		ready_view();
		return;
	}

	if(can_change_area && _input.is_input_down(app::input::right)) {

		automap_interface.next();
		ready_view();
		return;
	}
}

void pause::draw(
	ldv::screen& _screen,
	int /*_fps*/
) {

	view.draw(_screen);
	map_representation.draw(_screen);
}

void pause::ready_view() {

	const auto& area=automap_interface.get();

	//Get all cells for the current area and filter them with the
	//persistence layer. If we can see it then it has been discovered.
	//and should be drawn.

	std::vector<const app::map_cell*> cells;
	for(const auto& cell : area.cells) {

#ifdef IS_DEBUG_BUILD
		if(display_all_maps || persistence.has(app::pergr_automap, cell.id)) {

			cells.push_back(&cell);
		}

		lm::log(logger).debug()<<"map ready with "<<cells.size()<<" cells\n";
#else
		if(persistence.has(app::pergr_automap, cell.id)) {

			cells.push_back(&cell);
		}

#endif
	}

	map_representation.clear();
	if(area.must_draw_map) {

		ready_map_view(cells);
	}

	ready_map_name(cells);

}

void pause::ready_map_view(
	const std::vector<const app::map_cell*>& _cells
) {


	for(const auto& cell : _cells) {

		ready_room(*cell);
	}

	//only distant parts of the map (from 0.0) may be available, so in 
	//order to make then center ok we trim the inner distances.
	map_representation.trim();

#ifdef IS_DEBUG_BUILD

	lm::log(logger).debug()<<"map ready with "<<map_representation.size()<<" group nodes\n";
	lm::log(logger).debug()<<"map view position: "<<map_representation.get_view_position()<<std::endl;

#endif

	auto center_box=view.get_by_id("automap_center_box");
	map_representation.align(
		*center_box, 
		{
			ldv::representation_alignment::h::center,
			ldv::representation_alignment::v::center
		}
	);

#ifdef IS_DEBUG_BUILD

	lm::log(logger).debug()<<"map view position after centering: "<<map_representation.get_view_position()<<std::endl;

#endif
}

void pause::ready_room(
	const app::map_cell& _cell
) {

	const int wu=8, hu=8, wallw=1;

	int x=_cell.x*wu,
	    y=_cell.y*hu;
	unsigned w=_cell.w*wu,
		h=_cell.h*hu;

	auto wall_color=!persistence.has(app::pergr_automap, _cell.id)
			? wall_incomplete
			: persistence.get(app::pergr_automap, _cell.id)==app::am_discovered
				? wall_incomplete
				: wall_complete;

	auto fill_color=game_session.current_map_id==_cell.id
		? current_fill
		: regular_fill;

	//first off, the base with the wall colors...
	map_representation.insert(
		new ldv::box_representation({x, y, w, h}, wall_color)
	); 

	//then the "inner" colors... The fill goes over the wall giving the
	//impression of 1px walls.
	x+=wallw; 
	y+=wallw;
	w-=2*wallw;
	h-=2*wallw;

	map_representation.insert(
		new ldv::box_representation({x, y, w, h}, fill_color)
	); 

	//And now the features...
	using app::map_feature;
	for(const auto& feature: _cell.features) {

		int featx=(_cell.x*wu)+(feature.x*wu),
		    featy=(_cell.y*hu)+(feature.y*hu);

		if(feature.flags & map_feature::exit_top) {

			map_representation.insert(
				new ldv::box_representation({featx+2, featy, wu/2, wallw}, fill_color)
			); 
		}

		if(feature.flags & map_feature::exit_bottom) {

			map_representation.insert(
				new ldv::box_representation({featx+2, featy+hu-1, wu/2, wallw}, fill_color)
			); 
		}

		if(feature.flags & map_feature::exit_right) {

			map_representation.insert(
				new ldv::box_representation({featx+wu-1, featy+2, wallw, hu/2}, fill_color)
			); 
		}

		if(feature.flags & map_feature::exit_left) {

			map_representation.insert(
				new ldv::box_representation({featx, featy+2, wallw, hu/2}, fill_color)
			); 
		}

		if(feature.flags & map_feature::gate) {

			map_representation.insert(
				new ldv::box_representation({featx+3, featy+3, 2*wallw, 2*wallw}, wall_color)
			); 
		}

		if(feature.flags & map_feature::ellipsis_horizontal) {

			featx++;
			for(int i=0; i<3; i++) {
				map_representation.insert(
					new ldv::box_representation({featx, featy+4, wallw, wallw}, wall_color)
				); 
				featx+=2;
			}
		}

		if(feature.flags & map_feature::ellipsis_vertical) {

			featy++;
			for(int i=0; i<3; i++) {
				map_representation.insert(
					new ldv::box_representation({featx+4, featy, wallw, wallw}, wall_color)
				); 
				featy+=2;
			}
		}
	}
}

void pause::ready_map_name(
	const std::vector<const app::map_cell*>& _cells
) {

	const auto& area=automap_interface.get();
	bool area_complete=_cells.size() == area.cells.size();
	for(const auto& cell : _cells) {

		bool room_complete=persistence.has(app::pergr_automap, cell->id)
			&& persistence.get(app::pergr_automap, cell->id)==app::am_complete;
		area_complete&=room_complete;
	}

	auto area_color=area_complete
		? ldv::rgba8(247, 226, 107, 255)
		: ldv::rgba8(255, 255, 255, 255);

	view.set_text("area_name", localization.get(area.localization_key));
	view.set_text_color("area_name", area_color);
	auto area_name=view.get_by_id("area_name");

	auto center_box=view.get_by_id("automap_center_box");
	area_name->align(
		*center_box,
		{
			ldv::representation_alignment::h::center,
			ldv::representation_alignment::v::inner_top
		}
	);
}
