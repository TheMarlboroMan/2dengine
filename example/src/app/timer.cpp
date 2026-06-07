#include "app/timer.h"
#include <sstream>

using namespace app;

timer::timer(
	bool _active,
	bool _keep_active,
	int _tag,
	int _target_tag,
	int _pre_ms
):
	tag{_tag},
	target_tag{_target_tag},
	active{_active},
	starting_active{active},
	keep_active_when_reset{_keep_active},
	timeout{(double)_pre_ms / 1000., 0.}
{ }

void timer::reset() {

	if(!keep_active_when_reset) {

		active=starting_active;
	}

	timeout.restart();
}

void timer::receive_signal() {

	const auto finished=timeout.is_finished();
	const auto was_active=active;

	active=!active;

	if(was_active) {

		finished 
			? timeout.reset()
			: timeout.pause();
		return;
	}

	finished 
		? timeout.restart()
		: timeout.resume();
}

bool timer::tic(
	ldtools::tdelta _delta
) {

	if(!active) {

		return false;
	}

	timeout.tic(_delta);
	if(timeout.is_finished()) {

		active=!active;
		timeout.pause();

		return true;
	}

	return false;
}


std::ostream& app::operator<<(
	std::ostream& _stream,
	const timer&
) {

	_stream<<"timer[]";
	return _stream;
}
