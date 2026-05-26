#include "d2d/collision/spatiable_box.h"

using namespace d2d::collision;

spatiable_box::spatiable_box(
	t_position _x,
	t_position _y,
	t_size _w, 
	t_size _h
):
	collision_box{_x, _y, _w, _h},
	previous_box{collision_box}
{}

spatiable_box::spatiable_box(
	d2d::collision::point _pt,
	t_size _w, 
	t_size _h
):
	collision_box{_pt, _w, _h},
	previous_box{collision_box}
{}

spatiable_box::spatiable_box(
	d2d::collision::box _box
):
	collision_box{_box},
	previous_box{collision_box}
{}

void spatiable_box::commit_box() {

	previous_box=collision_box;
}

void spatiable_box::rollback_box() {

	collision_box=previous_box;
}

bool spatiable_box::is_passable_edge(
	d2d::collision::box_edge _edge
) const {

	switch(_edge) {

		case box_edge::top: return !(edges & aabb_edges::top);
		case box_edge::bottom: return !(edges & aabb_edges::bottom);
		case box_edge::right: return !(edges & aabb_edges::left);
		case box_edge::left: return !(edges & aabb_edges::right);
		default: 
			return false; //as much as box_edge is an enum class g++ keeps complaining.
	}
}

std::ostream& d2d::collision::operator<<(
	std::ostream& _stream,
	const spatiable_box& _entity
) {

	_stream<<_entity.get_box()<<" (prev "<<_entity.get_previous_box()<<")";
	return _stream;
}
