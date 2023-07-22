#include "d2d/world/collision_tile.h"

using namespace d2d::world;

collision_tile::collision_tile(
	int _x, 
	int _y, 
	int _type,
	d2d::collision::shaper& _shaper
):
	x{_x},
	y{_y},
	type{_type},
	shaper{&_shaper}
{}

std::ostream& d2d::world::operator<<(
	std::ostream& _stream,
	const collision_tile& _tile
) {

	_stream<<"tile["<<_tile.x<<", "<<_tile.y<<" <"<<_tile.type<<">]";
	return _stream;
}
