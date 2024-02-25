#include "d2d/video/scenery_tile.h"

using namespace d2d::video;

scenery_tile::scenery_tile(
	int _x, 
	int _y,
	int _type
):
	x{_x}, y{_y}, type{_type}
{}

std::ostream& d2d::video::operator<<(
	std::ostream& _stream,
	const scenery_tile& _tile
) {

	_stream<<"sc-tile["<<_tile.x<<", "<<_tile.y<<" <"<<_tile.type<<">]";
	return _stream;
}
