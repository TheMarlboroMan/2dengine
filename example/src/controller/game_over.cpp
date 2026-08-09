#include "controller/game_over.h"
#include "app/input.h"
#include "app/definitions.h"

#include <lm/log.h>
#include <tools/string_utils.h>
#include <tools/i8n.h>
#include <ldtools/ttf_manager.h>

using namespace controller;

game_over::game_over(
	app::service_provider& _sp
)
:
	logger{_sp.get_logger()},
	//TODO:
	font{_sp.get_ttf_manager().get("credits_font", 8)},
	music_player{_sp.get_music_player()},
	savegame_manager{_sp.get_savegame_manager()},
	game_session{_sp.get_game_session()}
{

}

void game_over::awake(
	dfw::input&
) {

	savegame_manager.load();
	music_player.swap(app::music_silence, 100);

	for(std::size_t i=0; i<savegame_manager.size(); i++) {

		if(game_session.savegame_file==savegame_manager.get(i).filename) {

			lm::log(logger).debug()<<"will erase savegame on slot "<<i<<"\n";
			savegame_manager.erase(i);
			return;
		}
	}

	lm::log(logger).debug()<<"did not find slot matching '"<<game_session.savegame_file<<"', will not remove\n";
}

void game_over::loop(
	dfw::input& _input,
	const dfw::loop_iteration_data&
) {

	if(_input().is_exit_signal()) {

		set_leave(true);
		return;
	}

	//TODO: CAN WE LEAVE? A CERTAIN AMOUNT OF TIME SHOULD PASS.
	if(_input.is_input_down(app::input::escape)) {

		pop_state();
		return;
	}
}

void game_over::draw(
	ldv::screen& _screen,
	int
) {

	_screen.clear(ldv::rgba8(32, 0, 0, 255));
}

