#include "app/timed_trap.h"

using namespace app;

timed_trap::timed_trap(
	d2d::collision::point _pt, 
	types _type, 
	bool _active,
	int _tag,
	int _pre_ms,
	int _harm_ms,
	int _post_ms
):
	ent{_pt, 0, 0},
	type{_type},
	tag{_tag},
	active{_active},
	pre_s{(float)_pre_ms / 1000.f},
	harm_s{(float)_harm_ms / 1000.f},
	post_s{(float)_post_ms / 1000.f},
	timeout{pre_s}
{
	switch(type) {

		case types::fire:

			ent.set_w(fire_w);
			ent.set_h(fire_h);
		break;
	}
}


int timed_trap::tic(
	float _delta
) {

	timeout.tic(_delta);
	if(!timeout.is_finished()) {

		return 0;
	}

	//What would our next state be???
	switch(state) {

		case states::pre:
			state=states::harm;
			timeout.target(harm_s).restart();
			return 1; //Will start sound!

		case states::harm:
			state=states::post;
			timeout.target(post_s).restart();
			return -1; //will stop sound...
		
		case states::post:
			state=states::pre;
			timeout.target(pre_s).restart();
			return 0; //will do nothing.
	}

	return 0;
}


std::ostream& app::operator<<(
	std::ostream& _stream,
	const timed_trap& _trap
) {

	_stream<<"timed_trap["<<_trap.ent<<"]";
	return _stream;
}
