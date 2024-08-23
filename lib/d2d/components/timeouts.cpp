#include <d2d/components/timeouts.h>

using namespace d2d::components;

timeout::timeout(
	float _max,
	float _timer,
	bool _paused
):
	timer{_timer==-1.0f ? 0.f : _timer},
	max{_max},
	paused{_paused}
{}

timeout& timeout::tic(
	float _delta
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
	float _value
) {

	max=_value;
	return *this;
}

timeout& timeout::reset() {

	timer=0.f;
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

float timeout::get() const {

	return timer;
}

float timeout::get_max() const {

	return max;
}

timeout& timeout::set(
	float _val
) {

	timer=_val;
	return *this;
}

////////////////////////////////

void timeouts::tic(
	float _delta
) {

	for(auto& pair : data) {

		pair.second.tic(_delta);
	}
}

void timeouts::tic(
	int _id,
	float _delta
) {

	data.at(_id).tic(_delta);
}

timeouts& timeouts::add(
	int _id,
	float _max_value,
	float _value,
	bool _paused
) {

	float val=_value==-1.0f
		? 0.f
		: _value;

	data.try_emplace(_id, _max_value, val, _paused);
	return *this;
}

float timeouts::get(
	int _id
) const {

	return data.at(_id).get();
}

float timeouts::get_max(
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
	float _value
) {

	return data.at(_id).set(_value);
}

timeout& timeouts::reset(
	int _id
) {

	return data.at(_id).reset();
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
