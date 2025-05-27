#include "app/entity.h"

using namespace app;

entity::entity(
	int _x,
	int _y,
	int _w, 
	int _h
):
	collision_box{{(double)_x, (double)_y}, _w, _h},
	previous_box{collision_box}
{}

entity::entity(
	d2d::collision::point _pt,
	int _w, 
	int _h
):
	collision_box{_pt, _w, _h},
	previous_box{collision_box}
{}

entity::entity(
	d2d::collision::box _box
):
	collision_box{_box},
	previous_box{collision_box}
{}

void entity::commit_box() {

	previous_box=collision_box;
}

void entity::rollback_box() {

	collision_box=previous_box;
}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const entity& _entity
) {

	_stream<<_entity.get_box()<<" (prev "<<_entity.get_previous_box()<<")";
	return _stream;
}
