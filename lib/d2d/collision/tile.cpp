#include "d2d/collision/tile.h"
#include <stdexcept>

using namespace d2d::collision;

tile::tile(
	int _x, 
	int _y, 
	int _type,
	d2d::collision::shaper& _shaper,
	const d2d::collision::tile_implementation& _cimpl
):
	x{_x},
	y{_y},
	type{_type},
	shaper{&_shaper},
	cimpl{&_cimpl}
{}

std::ostream& d2d::collision::operator<<(
	std::ostream& _stream,
	const tile& _tile
) {

	_stream<<"tile["<<_tile.x<<", "<<_tile.y<<" <"<<_tile.type<<">]";
	return _stream;
}
