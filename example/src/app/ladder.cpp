#include "app/ladder.h"
#include "app/definitions.h"

using namespace app;

ladder::ladder(
	d2d::collision::point _pt,
	int _h,
	int _type
):
	d2d::collision::motion_axis_limiter(
		{_pt, app::tile_w, _h},
		d2d::collision::motion_axis_limiter::axes::y,
		d2d::collision::motion_axis_limiter::alignments::center
	),
	type{_type}
{}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const ladder& _ladder
) {

	_stream<<"ladder["<<_ladder.get_box()<<"]";
	return _stream;
}
