#include "app/linear_monster.h"

using namespace app;

const double linear_monster::scorpion_velocity{20.};

linear_monster::linear_monster(
	d2d::collision::point _pt,
	int _type,
	bool _facing_right
):
	ent{ {_pt}, 0, 0},
	velocity{0., 0.},
	type{_type},
	facing{_facing_right ? app::faces::right : app::faces::left}
{

	switch(type) {

		case scorpion:

			ent.set_w(scorpion_w);
			ent.set_h(scorpion_h);
			velocity.x=facing==app::faces::right 
				? scorpion_velocity
				: -scorpion_velocity;
		break;
	}
}

void linear_monster::reverse() {

	facing=app::faces::right==facing
		? app::faces::left
		: app::faces::right;

	velocity.x=-velocity.x;
}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const linear_monster& _monster
) {

	_stream<<"linear_monster["<<_monster.ent.get_origin()<<"]";
	return _stream;
}
