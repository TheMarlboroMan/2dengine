#include "app/trap.h"
#include <sstream>

using namespace app;

trap::trap(
	d2d::collision::point _pt, 
	types _type, 
	bool _active,
	bool _keep_active,
	int _tag
):
	ent{_pt, 0, 0},
	type{_type},
	tag{_tag},
	active{_active},
	starting_active{active},
	keep_active_when_reset{_keep_active}
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

void trap::reset() {

	if(!keep_active_when_reset) {

		active=starting_active;
	}

}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const trap& _trap
) {

	_stream<<"trap["<<_trap.ent<<"]";
	return _stream;
}
