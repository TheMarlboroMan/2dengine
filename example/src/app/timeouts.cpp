#include <app/timeouts.h>

using namespace app;

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
	float _value
) {

	float val=_value==-1 
		? _max_value
		: _value;

	data[_id]={val, _max_value};
	return *this;
}

timeouts& timeouts::reset(
	int _id
) {

	data[_id].reset();
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
