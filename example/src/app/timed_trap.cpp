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
	tag{_tag},
	active{_active},
	ent{_pt, 0, 0},
	type{_type},
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


void timed_trap::tic(
	float _delta
) {

	timeout.tic(_delta);
	if(!timeout.is_expired()) {

		return;
	}

	switch(state) {

		case states::pre:
			state=states::harm;
			timeout.reset().target(harm_s);
			return;

		case states::harm:
			state=states::post;
			timeout.reset().target(post_s);
			return;
		
		case states::post:
			state=states::pre;
			timeout.reset().target(pre_s);
			return;
	}
}


std::ostream& app::operator<<(
	std::ostream& _stream,
	const timed_trap& _trap
) {

	_stream<<"timed_trap["<<_trap.ent<<"]";
	return _stream;
}
