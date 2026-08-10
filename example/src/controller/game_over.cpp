#include "controller/game_over.h"
#include "app/input.h"
#include "app/definitions.h"

#include <lm/log.h>
#include <tools/string_utils.h>
#include <tools/i8n.h>
#include <ldtools/ttf_manager.h>
#include <tools/file_utils.h>
#include <tools/json.h>
#include <tools/i8n.h>
#include <ldv/ttf_representation.h>

using namespace controller;

game_over::game_over(
	app::service_provider& _sp
)
:
	logger{_sp.get_logger()},
	music_player{_sp.get_music_player()},
	savegame_manager{_sp.get_savegame_manager()},
	game_session{_sp.get_game_session()},
	timer{2., 0.} //we will take a couple of seconds to appear.
{

	layout.map_font(
		"game_over_big_font", 
		_sp.get_ttf_manager().get("game_over_big_font", 24)
	);

	layout.map_font(
		"game_over_font", 
		_sp.get_ttf_manager().get("game_over_font", 8)
	);

	const std::string layout_path=_sp.get_env().build_app_path("resources/layout/views.json");
	auto document=tools::parse_json_string(tools::dump_file(layout_path));
	layout.parse(document["game_over"]);

	const auto& i8n=_sp.get_localization();
	layout.set_text("game_over", i8n.get("game_over-main"));
	layout.set_text("progress_text", i8n.get("game_over-text"));

	auto text=layout.get_by_id("game_over");
	auto center_box=layout.get_by_id("cover_box");
	text->align(
		*center_box, 
		{
			ldv::representation_alignment::h::center,
			ldv::representation_alignment::v::none
		}
	);
}

void game_over::awake(
	dfw::input&,
	int
) {

	//Restore cover block alpha.
	layout.set_alpha("cover_box", 255);
	timer.restart();

	savegame_manager.load();
	music_player.swap(app::music_silence, 100);

	for(std::size_t i=0; i<savegame_manager.size(); i++) {

		if(game_session.savegame_file==savegame_manager.get(i).filename) {

			//Borderline case that does not happen in-game: the slot is not 
			//actually saved and somehow the player has managed to game-over on
			//the very first screen before the game was actually saved!
			const auto& slot=savegame_manager.get(i);
			if(slot.new_game) {

				lm::log(logger).debug()<<"savegame on slot "<<i<<" is empty, will not erase\n";
				return;
			}

			lm::log(logger).debug()<<"will erase savegame on slot "<<i<<"\n";
			savegame_manager.erase(i);
			return;
		}
	}

	lm::log(logger).debug()<<"did not find slot matching '"<<game_session.savegame_file<<"', will not remove\n";
}

void game_over::loop(
	dfw::input& _input,
	const dfw::loop_iteration_data& _lid
) {

	timer.tic(_lid.delta);

	if(_input().is_exit_signal()) {

		set_leave(true);
		return;
	}

	if(timer.is_finished() && _input.is_input_down(app::input::jump)) {

		pop_state();
		return;
	}
	else {

		double alpha=255. - (255. * timer.get_linear());
		layout.set_alpha("cover_box", (int)alpha);
	}
}

void game_over::draw(
	ldv::screen& _screen,
	int
) {

	layout.draw(_screen);
}

