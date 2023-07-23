#include "controller/main.h"

using namespace controller;

main::main()
{
}

void main::awake(
	dfw::input& /*input*/
) {
}

void main::slumber(
	dfw::input& /*input*/
) {

}

void main::loop(
	dfw::input& _input,
	const dfw::loop_iteration_data& _lid
) {

/*
	if(_input().is_exit_signal()) {
		set_leave(true);
		return;
	}

	if(_input.is_input_down(input::escape)) {

		game_music.disable();
		set_state(t_states::state_main_menu);
		return;
	}
*/
}

void main::draw(
	ldv::screen& _screen,
	int _fps
) {

}
