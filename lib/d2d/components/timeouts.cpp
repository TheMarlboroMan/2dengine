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
	}

	return *this;
}

timeout& timeout::reset() {

	timer=0.f;
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

bool timeout::is_counting() const {

	return timer != max;
}

bool timeout::is_expired() const {

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

bool timeouts::is_expired(
	int _id
) const {

	return data.at(_id).is_expired();
}

bool timeouts::is_counting(
	int _id
) const {

	return data.at(_id).is_counting();
}

timeouts& timeouts::set(
	int _id,
	float _value
) {

	data.at(_id).set(_value);
	return *this;
}

timeouts& timeouts::reset(
	int _id
) {

	data.at(_id).reset();
	return *this;
}

timeouts& timeouts::pause(
	int _id
) {

	data.at(_id).pause();
	return *this;
}

timeouts& timeouts::resume(
	int _id
) {

	data.at(_id).resume();
	return *this;
}

bool timeouts::is_paused(
	int _id
) const {

	return data.at(_id).is_paused();
}
