#include <d2d/components/timeout.h>

using namespace d2d::components;

timeout::timeout(
	ldtools::tdelta _max,
	ldtools::tdelta _timer,
	bool _paused
):
	timer{_timer},
	max{_max},
	paused{_paused}
{}

timeout& timeout::tic(
	ldtools::tdelta _delta
) {

	if(paused) {

		return *this;
	}

	timer+=_delta;
	if(timer >= max) {

		timer=max;
		pause();
	}

	return *this;
}

timeout& timeout::target(
	ldtools::tdelta _value
) {

	max=_value;
	return *this;
}

timeout& timeout::reset() {

	timer=0.;
	return *this;
}

timeout& timeout::restart() {

	reset();
	resume();
	return *this;
}

timeout& timeout::pause() {

	paused=true;
	return *this;
}

timeout& timeout::resume() {

	paused=false;
	return *this;
}

bool timeout::is_paused() const {

	return paused;
}

bool timeout::is_running() const {

	return timer != max;
}

bool timeout::is_finished() const {

	return timer == max;
}

ldtools::tdelta timeout::get() const {

	return timer;
}

ldtools::tdelta timeout::get_max() const {

	return max;
}

timeout& timeout::set(
	ldtools::tdelta _val
) {

	timer=_val;
	return *this;
}

////////////////////////////////

void timeouts::tic(
	ldtools::tdelta _delta
) {

	for(auto& pair : data) {

		pair.second.tic(_delta);
	}
}

void timeouts::tic(
	int _id,
	ldtools::tdelta _delta
) {

	data.at(_id).tic(_delta);
}

timeouts& timeouts::add(
	int _id,
	ldtools::tdelta _max_value,
	ldtools::tdelta _value,
	bool _paused
) {

	data.try_emplace(_id, _max_value, _value, _paused);
	return *this;
}

ldtools::tdelta timeouts::get(
	int _id
) const {

	return data.at(_id).get();
}

ldtools::tdelta timeouts::get_max(
	int _id
) const {

	return data.at(_id).get_max();
}

bool timeouts::is_finished(
	int _id
) const {

	return data.at(_id).is_finished();
}

bool timeouts::is_running(
	int _id
) const {

	return data.at(_id).is_running();
}

timeout& timeouts::set(
	int _id,
	ldtools::tdelta _value
) {

	return data.at(_id).set(_value);
}

timeout& timeouts::reset(
	int _id
) {

	return data.at(_id).reset();
}

timeout& timeouts::target(
	int _id,
	ldtools::tdelta _value
) {

	return data.at(_id).target(_value);
}

timeout& timeouts::restart(
	int _id
) {

	return data.at(_id).restart();
}

timeouts& timeouts::reset() {

	for(auto& node : data) {

		node.second.reset();
	}
	return *this;
}

timeouts& timeouts::pause() {

	for(auto& node : data) {

		node.second.pause();
	}
	return *this;
}

timeouts& timeouts::restart() {

	for(auto& node : data) {

		node.second.restart();
	}
	return *this;
}

timeout& timeouts::pause(
	int _id
) {

	return data.at(_id).pause();
}

timeout& timeouts::resume(
	int _id
) {

	return data.at(_id).resume();
}

bool timeouts::is_paused(
	int _id
) const {

	return data.at(_id).is_paused();
}

std::ostream& d2d::components::operator<<(
	std::ostream& _stream,
	const timeout& _timeout
) {

	_stream<<"timeout "<<_timeout.get()<<" / "<<_timeout.get_max()<<" [";

	if(_timeout.is_running()) {

		_stream<<"running]";
	}
	else if(_timeout.is_finished()) {

		_stream<<"finished]";
	}

	if(_timeout.is_paused()) {

		_stream<<" [paused]";
	}

	return _stream;
}
