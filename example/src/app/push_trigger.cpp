#include "app/push_trigger.h"
#include <iostream>
using namespace app;

push_trigger::push_trigger(
	const d2d::collision::box& _box,
	d2d::motion::motion_vector _vec
):
	ent{_box},
	velocity{_vec},
	timeout{0.5, 0., true} 
{}

bool push_trigger::is_active() const {

	return timeout.is_paused();
}

void push_trigger::activate() {

	timeout.restart();
}

void push_trigger::tic(
	ldtools::tdelta _delta
) {

	if(is_active()) {

		return;
	}

	timeout.tic(_delta);
	if(timeout.is_finished()) {

		timeout.pause();
	}
}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const push_trigger& _trigger
) {

	_stream<<"push_trigger["<<_trigger.ent<<"]";
	return _stream;
}

