#include "app/platform_block.h"

using namespace app;

platform_block::platform_block(
	int _x,
	int _y,
	int _w 
):
	collision_box{{(double)_x, (double)_y}, _w, 12}
{}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const platform_block& _block
) {

	_stream<<"platform_block["<<_block.get_box()<<"]";
	return _stream;
}
