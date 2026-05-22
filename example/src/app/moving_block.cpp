#include "app/moving_block.h"
#include <d2d/motion/mover.h>
#include <ldt/vector_2d.h>

using namespace app;

moving_block::moving_block(
	const d2d::collision::box& _box,
	int _tag,
	int _first_nodeid,
	int _type,
	bool _active,
	bool _solid
):
	ent{_box},
	state{states::receiving},
	tag{_tag},
	first_nodeid{_first_nodeid},
	next_id{_first_nodeid},
	type{_type},
	active{_active}, 
	initial_active{active},
	solid{_solid},
	target{0., 0.},
	initial_position{ent.get_origin()},
	timeout{0., 0, true}
{ }

void moving_block::reset() {

	state=states::arrived; //this will make it seek the first node again.
	target={0., 0.};
	active=initial_active;
	ent.set_origin(initial_position);
	timeout.reset();
	next_id=first_nodeid;
}

void moving_block::set_target(
	const d2d::collision::point& _target,
	int _velocity,
	int _ms_waiting,
	int _next_id //the id of the next waypoint after the current one is done!
) {

	target=_target;
	next_id=_next_id;
	timeout.target((double)_ms_waiting / 1000.).restart();

	auto vec=ldt::vector_from_points(ent.get_origin(), target).normalize();
	ent.set_motion_vector(vec*(double)_velocity);

	state=states::waiting;
}

void moving_block::tic(
	ldtools::tdelta _delta,
	const d2d::motion::mover& _mover
) {

	if(!active) {

		return;
	}

	switch(state) {

		case states::receiving:
			return;
		case states::waiting:

			timeout.tic(_delta);
			if(timeout.is_finished()) {

				state=states::in_route;
			}
			return;
		case states::in_route:

			_mover.apply(ent, _delta);
			if(is_on_or_beyond_target()) {

				ent.set_origin(target);
				state=states::arrived;
			}
			return;
		case states::arrived:
			return;
		case states::invalid:
			return;
	}
}

void moving_block::invalidate() {

	state=states::invalid;
}

bool moving_block::is_on_or_beyond_target() const {

	if(target==ent.get_origin()) {

		return true;
	}

	const auto vector=ent.get_motion_vector();
	const auto current=ent.get_origin();
	if(vector.x) {

		if(vector.x > 0. && current.x > target.x) {

			return true;
		}

		if(vector.x < 0. && current.x < target.x) {

			return true;
		}
	}

	if(vector.y) {

		if(vector.y > 0. && current.y > target.y) {

			return true;
		}

		if(vector.y < 0. && current.y < target.y) {

			return true;
		}
	}

	return false;
}

void moving_block::activate() {

	active=true;
}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const moving_block& _block
) {

	_stream<<"moving_block["<<_block.ent.get_box()<<"]";
	return _stream;
}
