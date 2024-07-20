#include "controller/controller_states.h"
#include "controller/pause.h"
#include "app/input.h"

using namespace controller;

pause::pause(
	app::service_provider& _sp
):
	sp{_sp},
	env{_sp.get_env()},
	logger{_sp.get_logger()},
	inventory{_sp.get_inventory()}
{

}

void pause::awake(
	dfw::input& /*input*/
) {

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
}

void pause::draw(
	ldv::screen& _screen,
	int /*_fps*/
) {

	//TODO: Sure, how do I know if I must draw the time and shit? Maybe it's
	//just checking that the values are not -1
	//or maybe we have to also pass along the skill, which should be part
	//of a ... game_session? or just the inventory, to be honest xD
	
	//TODO: We could use a layout for keys and numbers and shit, right?
	//TODO: Drawing the map would be MUCH MUCH more fun.
	//TODO: How do we know the current map id we are at¿ Again, the inventory
	//or the game session xD!

	_screen.clear({0, 0, 0, 0});
}

