#include "app/timed_trap.h"
#include <sstream>

using namespace app;

timed_trap::timed_trap(
	d2d::collision::point _pt, 
	types _type, 
	bool _active,
	bool _keep_active,
	int _tag,
	int _pre_ms,
	int _harm_ms,
	int _post_ms
):
	ent{_pt, 0, 0},
	type{_type},
	tag{_tag},
	active{_active},
	starting_active{active},
	keep_active_when_reset{_keep_active},
	pre_s{(double)_pre_ms / 1000.},
	harm_s{(double)_harm_ms / 1000.},
	post_s{(double)_post_ms / 1000.},
	timeout{pre_s}
{
	switch(type) {

		case types::fire:
			ent.set_w(fire_w);
			ent.set_h(fire_h);
		break;
		case types::spikes:
			ent.set_w(spikes_w);
			ent.set_h(spikes_h);
		break;
	}
}

void timed_trap::reset() {

	if(!keep_active_when_reset) {

		active=starting_active;
	}

	state=states::pre;
	timeout.target(pre_s).restart();
}

void timed_trap::tic(
	ldtools::tdelta _delta
) {

	if(!active) {

		return;
	}

	timeout.tic(_delta);
	if(!timeout.is_finished()) {

		return;
	}

#ifdef IS_DEBUG_BUILD
	int count=0;
#endif

	//TODO: This even needed anymore????
	//Run this while the next state has no timeout, so we can skip empty times
	//without another tic taking place.
	while(true) {

		switch(state) {

			case states::pre:
				state=states::harm;
				timeout.target(harm_s).restart();
			break;

			case states::harm:
				state=states::post;
				timeout.target(post_s).restart();
			break;
			
			case states::post:
				state=states::pre;
				timeout.target(pre_s).restart();
			break;
		}

		if(0.!=timeout.get_max()) {

			return;
		}

#ifdef IS_DEBUG_BUILD

		if(++count==3) {

			std::stringstream ss;
			ss<<"bad timed trap, not all times can be zero for "<<*this;
			throw std::runtime_error(ss.str());
		}
#endif
	}
}


std::ostream& app::operator<<(
	std::ostream& _stream,
	const timed_trap& _trap
) {

	_stream<<"timed_trap["<<_trap.ent<<" pre:"<<_trap.pre_s<<" harm:"<<_trap.harm_s<<" post:"<<_trap.post_s<<"]";
	return _stream;
}
