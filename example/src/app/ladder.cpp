#include "app/ladder.h"
#include "app/definitions.h"

using namespace app;

ladder::ladder(
	int _x,
	int _y,
	int _h
):
	d2d::collision::motion_axis_limiter(
		{{(double)_x, (double)_y}, app::tile_w, _h},
		d2d::collision::motion_axis_limiter::axes::y,
		d2d::collision::motion_axis_limiter::alignments::center
	)
{}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const ladder& _ladder
) {

	_stream<<"ladder["<<_ladder.get_box()<<"]";
	return _stream;
}
