#include "controller/leave_ending.h"
#include "app/input.h"

#include <lm/log.h>
#include <ldv/color.h>

using namespace controller;

leave_ending::leave_ending(
	app::service_provider& _sp
)
:
	log{_sp.get_logger()},
	timeout{3., 0.}
{ }

void leave_ending::awake(
	dfw::input&
) {

}

void leave_ending::loop(
	dfw::input& _input,
	const dfw::loop_iteration_data& _lid
) {

	if(_input().is_exit_signal()) {

		set_leave(true);
		return;
	}

	timeout.tic(_lid.delta);
	if(timeout.is_finished()) {

		//The current state is replaced with the credits, as this one replaced
		//the game, so that when we pop from the credits we end up in the 
		//main menu again.
		set_state(state_credits);
		return;
	}
}

void leave_ending::draw(
	ldv::screen& _screen,
	int
) {

	_screen.clear(ldv::rgba8(0, 0, 0, 255));
}
