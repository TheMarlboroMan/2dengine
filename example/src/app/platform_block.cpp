#include "app/platform_block.h"

using namespace app;

platform_block::platform_block(
	d2d::collision::point _pt,
	types _type
):
	collision_box{_pt, w, h},
	type{_type}
{}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const platform_block& _block
) {

	_stream<<"platform_block["<<_block.get_box()<<"]";
	return _stream;
}
