#include <d2d/components/timeouts.h>

using namespace d2d::components;

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
	bool _active
) {

	float val=_value==-1 
		? _max_value
		: _value;

	data[_id]={val, _max_value, _active};
	return *this;
}

float timeouts::get(
	int _id
) const {

	return data.at(_id).timer;
}


bool timeouts::is_expired(
	int _id
) const {

	return data.at(_id).timer == 0.f;
}

bool timeouts::is_counting(
	int _id
) const {

	return data.at(_id).timer != 0.f;
}

timeouts& timeouts::set(
	int _id,
	float _value
) {

	data[_id].timer=_value;
	return *this;
}

timeouts& timeouts::reset(
	int _id
) {

	data[_id].reset();
	return *this;
}

timeouts& timeouts::pause(
	int _id
) {

	data[_id].pause();
	return *this;
}

timeouts& timeouts::resume(
	int _id
) {

	data[_id].resume();
	return *this;
}

bool timeouts::is_paused(
	int _id
) const {

	return data.at(_id).paused;
}

void timeouts::timeout::tic(
	float _delta
) {
	if(paused) {

		return;
	}

	timer-=_delta;
	if(timer < 0.f) {

		timer=0.f;
	}
}

void timeouts::timeout::reset() {

	timer=max;
}

void timeouts::timeout::pause() {

	paused=true;
}

void timeouts::timeout::resume() {

	paused=false;
}
