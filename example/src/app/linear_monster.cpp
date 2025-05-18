#include "app/linear_monster.h"

using namespace app;

const double linear_monster::scorpion_velocity{20.};
const double linear_monster::snake_velocity{30.};
const double linear_monster::bat_velocity{15.};

linear_monster::linear_monster(
	d2d::collision::point _pt,
	types _type,
	bool _facing_right,
	boundaries _bounds
):
	ent{ {_pt}, 0, 0},
	velocity{0., 0.},
	type{_type},
	facing{_facing_right ? app::faces::right : app::faces::left},
	starting_pos{_pt},
	starting_face{facing},
	bounds{_bounds}
{

	switch(type) {

		case types::scorpion:

			ent.set_w(scorpion_w);
			ent.set_h(scorpion_h);
		break;
		case types::snake:

			ent.set_w(snake_w);
			ent.set_h(snake_h);
		break;
		case types::bat:

			ent.set_w(bat_w);
			ent.set_h(bat_h);
		break;
	}

	reset_velocity();
}

void linear_monster::tic(
	ldtools::tdelta _delta,
	d2d::motion::mover _mover
) {

	_mover.apply(ent, velocity, _delta);

	double pos=is_horizontal_movement() 
		? ent.get_x()
		: ent.get_y();

	double dimension=is_horizontal_movement() 
		? ent.get_w()
		: ent.get_h();

	if(pos <= bounds.lower || pos+dimension >= bounds.upper) {

		reverse();
		return;
	}
}

void linear_monster::reverse() {

	facing=app::faces::right==facing
		? app::faces::left
		: app::faces::right;

	velocity*=-1.;
}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const linear_monster& _monster
) {

	_stream<<"linear_monster["<<_monster.ent.get_origin()<<"]";
	return _stream;
}

bool linear_monster::is_horizontal_movement() const {

	switch(type) {

		case types::scorpion:
		case types::snake:
			return true;

		case types::bat:
			return false;
	}

	return false;
}

void linear_monster::reset() {

	ent.set_origin(starting_pos);
	facing=starting_face;
	reset_velocity();
}

void linear_monster::reset_velocity() {

	switch(type) {

		case types::scorpion:

			velocity.x=facing==app::faces::right 
				? scorpion_velocity
				: -scorpion_velocity;
		break;
		case types::snake:

			velocity.x=facing==app::faces::right 
				? snake_velocity
				: -snake_velocity;
		break;
		case types::bat:

			velocity.x=facing==app::faces::right 
				? bat_velocity
				: -bat_velocity;
		break;
	}
}
