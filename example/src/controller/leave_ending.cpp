#include "controller/leave_ending.h"
#include "app/input.h"

#include <lm/log.h>

using namespace controller;

leave_ending::leave_ending(
	app::service_provider& _sp
)
:
	log{_sp.get_logger()}
{ }

void leave_ending::awake(
	dfw::input&
) {

}

void leave_ending::loop(
	dfw::input& _input,
	const dfw::loop_iteration_data&
) {

	if(_input().is_exit_signal()) {

		set_leave(true);
		return;
	}
}

void leave_ending::draw(
	ldv::screen&,
	int
) {

}
