#include "app/solid_block.h"

using namespace app;

solid_block::solid_block(
	int _x,
	int _y,
	int _w, 
	int _h
):
	entity(_x, _y)
{

	auto& box=get_box();
	box.w=_w;
	box.h=_h;
}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const solid_block& _block
) {

	const auto& box=_block.get_box();
	_stream<<box.origin.x<<","<<box.origin.y<<" "<<box.w<<"x"<<box.h;
	return _stream;
}
