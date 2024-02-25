#include "d2d/collision/shaper_default.h"
#include "d2d/collision/exception.h"

#include <string>

using namespace d2d::collision;

shaper_default::shaper_default(
	int _tile_w,
	int _tile_h
):
	tile_w{_tile_w}, tile_h{_tile_h},
	full_box{0, 0, tile_w, tile_h},
	half_box_wide{0, 0, tile_w, tile_h / 2},
	half_box_tall{0, 0, tile_w / 2, tile_h},
	quarter_box{0, 0, tile_w / 2, tile_h / 2}
{}

d2d::collision::box& shaper_default::get_box(
	int _type,
	int _x,
	int _y
) {

	switch(_type) {

		case tile_full:
			full_box.origin.x=_x*tile_w;
			full_box.origin.y=_y*tile_h;
			return full_box;
		case tile_half_bottom:
			half_box_wide.origin.x=_x*tile_w;
			half_box_wide.origin.y=_y*tile_h;
			return half_box_wide;
		case tile_half_top:
			half_box_wide.origin.x=_x*tile_w;
			half_box_wide.origin.y=_y*tile_h+(tile_h / 2);
			return half_box_wide;
		case tile_half_left:
			half_box_tall.origin.x=_x*tile_w;
			half_box_tall.origin.y=_y*tile_h;
			return half_box_tall;
		case tile_half_right:
			half_box_tall.origin.x=_x*tile_w+(tile_w / 2);
			half_box_tall.origin.y=_y*tile_h;
			return half_box_tall;
		case tile_quarter_bottom_left:
			quarter_box.origin.x=_x*tile_w;
			quarter_box.origin.y=_y*tile_h;
			return quarter_box;
		case tile_quarter_bottom_right:
			quarter_box.origin.x=_x*tile_w+(tile_w / 2);
			quarter_box.origin.y=_y*tile_h;
			return quarter_box;
		case tile_quarter_top_left:
			quarter_box.origin.x=_x*tile_w;
			quarter_box.origin.y=_y*tile_h+(tile_h / 2);
			return quarter_box;
		case tile_quarter_top_right:
			quarter_box.origin.x=_x*tile_w+(tile_w / 2);
			quarter_box.origin.y=_y*tile_h+(tile_h / 2);
			return quarter_box;
		default:
			throw exception(std::string{"unknown tile type"}+std::to_string(_type));
	}

	return full_box; //Please compiler, just shut up.
}
