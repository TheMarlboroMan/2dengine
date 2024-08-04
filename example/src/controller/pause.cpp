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
	sp{_sp},
	env{_sp.get_env()},
	logger{_sp.get_logger()},
	inventory{_sp.get_inventory()},
	localization{_sp.get_localization()},
	persistence{_sp.get_persistence()},
	game_session{_sp.get_game_session()},
	automap_interface{_sp.get_automap()},
	map_representation{{0,0}},
	wall_complete{ldv::rgba8(255, 255, 255, 255)},
	wall_incomplete{ldv::rgba8(157, 157, 157, 255)},
	regular_fill{ldv::rgba8(164, 100, 34, 255)},
	current_fill{ldv::rgba8(190, 38, 51, 255)}
{

	view.map_font(
		"pause_font",
		sp.get_ttf_manager().get("pause_font", 8)
	);

	view.map_texture("main", sp.get_video_resource_manager().get_texture(app::tex_tiles));

	const std::string layout_path=env.build_app_path("resources/layout/views.json");
	auto document=tools::parse_json_string(tools::dump_file(layout_path));
	view.parse(document["pause"]);
}

void pause::awake(
	dfw::input& /*input*/
) {

	timeout_passed=false;
	time_elapsed=0.0f;

	//Hide or show stuff according to game mode.
	view.set_visible("lives_icon", game_session.with_lives());
	view.set_visible("lives_value", game_session.with_lives());
	view.set_visible("time_value", game_session.with_timer());

	//Update the values to be shown...
	std::stringstream ss;
	ss<<" x "<<persistence.size(app::pergr_collectibles);
	view.set_text("treasure_value", ss.str());

	ss.str("");
	ss<<" x "<<inventory.yellow_keys;
	view.set_text("keys_value", ss.str());

	if(game_session.with_lives()) {

		ss.str("");
		ss<<" x "<<game_session.lives;
		view.set_text("lives_value", ss.str());
	}

	if(game_session.with_timer()) {

		lm::log(logger).debug()<<"game session seconds: "<<game_session.elapsed_seconds<<"\n";
		lm::log(logger).debug()<<"game clock seconds: "<<game_session.game_clock.get_seconds()<<"\n";

		ss.str("");
		tools::time t;
		auto seconds_elapsed=game_session.elapsed_seconds+game_session.game_clock.get_seconds();
		auto td=t.seconds_to_timedata(seconds_elapsed);
		ss<<t.time_to_string(td.hours, td.minutes, td.seconds);
		view.set_text("time_value", ss.str());
	}

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
	auto area_id=automap_interface.area_id_from_map_id(game_session.current_map_id);
	automap_interface.set(area_id);
	ready_map();
}

void pause::slumber(
	dfw::input& /*input*/
) {

}

void pause::loop(
	dfw::input& _input,
	const dfw::loop_iteration_data& _lid
) {

	if(!timeout_passed) {

		evaluate_timeout(_lid.delta);
	}

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

	if(_input.is_input_down(app::input::jump)) {

		lm::log(logger).debug()<<"toggling 'display_all_maps'\n";
		display_all_maps=!display_all_maps;
		ready_map();
	}
#endif

	if(_input.is_input_down(app::input::left)) {

		automap_interface.previous();
		ready_map();
		return;
	}

	if(_input.is_input_down(app::input::right)) {

		automap_interface.next();
		ready_map();
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

void pause::ready_map() {

	const auto& area=automap_interface.get();
	view.set_text("area_name", localization.get(area.localization_key));

	//Get all cells for the current area and filter them with the
	//persistence layer. If we can see it then it has been discovered.
	//and should be drawn.
	std::vector<const app::map_cell*> cells;
	for(const auto& cell : area.cells) {

#ifdef IS_DEBUG_BUILD
		if(display_all_maps || persistence.has(app::pergr_automap, cell.id)) {

			cells.push_back(&cell);
		}
#else
		if(persistence.has(app::pergr_automap, cell.id)) {
			cells.push_back(&cell);
		}
#endif
	}

	map_representation.clear();
	for(const auto& cell : cells) {

		ready_room(*cell);
	}

	auto center_box=view.get_by_id("automap_center_box");
	map_representation.align(
		*center_box, 
		{
			ldv::representation_alignment::h::center,
			ldv::representation_alignment::v::center
		}
	);

	auto area_name=view.get_by_id("area_name");
	area_name->align(
		*center_box,
		{
			ldv::representation_alignment::h::center,
			ldv::representation_alignment::v::inner_top
		}
	);
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

void pause::evaluate_timeout(
	float _delta
) {

	//Can only leave this controller after a second has passed, to prevent
	//crazy controller shifting.
	time_elapsed+=_delta;
	timeout_passed=time_elapsed >= 1.0f;

#ifdef IS_DEBUG_BUILD

	if(timeout_passed) {

		lm::log(logger).debug()<<"timeout passed, can leave controller now\n";
	}

#endif
}
