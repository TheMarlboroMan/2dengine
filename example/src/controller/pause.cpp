#include "controller/controller_states.h"
#include "controller/pause.h"
#include "app/input.h"

#include <ldtools/ttf_manager.h>
#include <tools/json.h>
#include <tools/file_utils.h>
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
	automap_interface{_sp.get_automap()},
	persistence{_sp.get_persistence()},
	map_representation{{100,100}} //To say something xD
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

	//TODO: Hide the kind of shit we don't need according to game_sesion.
	//
	//TODO: Select the current map and shit... This is actually interesting
	//because we need to find the AREA from the current map id. Not a lot
	//but well...
	//
	set_area_name();

	//TODO: use the automap to select the current area.
	//
	ready_map();
}

void pause::slumber(
	dfw::input& /*input*/
) {

}

void pause::loop(
	dfw::input& _input,
	const dfw::loop_iteration_data& /*_lid*/
) {

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

	//TODO: change map position and so on.
}

void pause::draw(
	ldv::screen& _screen,
	int /*_fps*/
) {

	view.draw(_screen);
	//TODO: Center when creating!
	std::cout<<map_representation.size()<<std::endl;
	map_representation.draw(_screen);
}

void pause::ready_map() {


	//TODO: We also have to set the map name, right? 
	//For that we would need a way to align the text, but that is not 
	//supported in the whole view, right?

	std::vector<const app::map_cell*> cells;

	//Get all cells for the current area and filter them witn the
	//persistence layer. If we can see it then it has been discovered.
	//and should be drawn.
	
	///TODO: NOt really zero!!!
	const auto& area=automap_interface.get(0);
	for(const auto& cell : area.cells) {

		if(persistence.has(app::pergr_automap, cell.id)) {

			cells.push_back(&cell);
		}
	}

	map_representation.clear();
	for(const auto& cell : cells) {

		ready_room(*cell);
	}
	
	//TODO: Center the representation xD

}

void pause::ready_room(
	const app::map_cell& _cell
) {

	const int wu=8, hu=8;

	//TODO: use blue borders when a room is "complete". How do we know when a room
	//is complete, btw??????? We use "persistence" for that. A status of a room is
	//0: unseen, 1: seen, 2: complete.
	
	//first off, the base with the wall colors...
	int x=_cell.x*wu,
	    y=_cell.y*hu,
	    w=_cell.w*wu,
		h=_cell.h*hu;

	map_representation.insert(
		new ldv::box_representation({x, y, w, h}, {255, 255, 255, 255})
	); 

	//then the "inner" colors, 2 pixels in.
	x+=2; 
	y+=2;
	w-=4;
	h-=4;
	map_representation.insert(
		new ldv::box_representation({x, y, w, h}, {128, 128, 128, 255})
	); 

	//And now the features...
	//TODO:
}

void pause::set_area_name() {

}
