#include "app/leaping_monster.h"

using namespace app;


leaping_monster::leaping_monster(
	d2d::collision::point _pt,
	types _type,
	int _rest_ms,
	int _force
):
	ent{ {_pt}, 0, 0},
	velocity{0., 0.},
	type{_type},
	spawn_y{ (int)_pt.y},
	leap_force{_force},
	//spawns starting the countdown.
	timeout{(float)_rest_ms / 1000., 0.f, false}
{

	switch(type) {

		case types::piranha:

			ent.set_w(piranha_w);
			ent.set_h(piranha_h);
		break;
	}
}

void leaping_monster::tic(
	float _delta,
	d2d::motion::mover _mover
) {

	if(!on_air) {

		timeout.tic(_delta);

		if(timeout.is_expired()) {

			velocity.y=leap_force;
			on_air=true;
		}

		return;
	}

	_mover.apply(ent, velocity, _delta);

	//Is the entity under the original position?
	if(ent.get_y() <= spawn_y) {

		timeout.reset();
		ent.set_y(spawn_y);
		velocity.y=0.;
		on_air=false;
	}
}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const leaping_monster& _monster
) {

	_stream<<"leaping_monster["<<_monster.ent.get_origin()<<"]";
	return _stream;
}

