#include "app/solid_block.h"

using namespace app;

solid_block::solid_block(
	int _x,
	int _y,
	int _w, 
	int _h
):
	collision_box{{(double)_x, (double)_y}, _w, _h}
{}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const solid_block& _block
) {

	_stream<<"solid_block["<<_block.get_box()<<"]";
	return _stream;
}
