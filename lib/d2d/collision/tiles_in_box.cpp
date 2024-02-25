#include <d2d/collision/tiles_in_box.h>
#include <iostream>

using namespace d2d::collision;

tiles_in_box::tiles_in_box(
	int _w,
	int _h
):
	tile_w{_w},
	tile_h{_h}
{}

std::vector<const d2d::collision::tile *> tiles_in_box::find(
	const d2d::collision::spatiable& _entity,
	const d2d::collision::tile_finder& _finder
) const {

	return find(_entity.get_box(), _finder);
}

std::vector<const d2d::collision::tile *> tiles_in_box::find(
	const d2d::collision::box& _entity,
	const d2d::collision::tile_finder& _finder
) const {

	std::vector<const d2d::collision::tile *> result;

	//These will be directly in tile values.
	const auto origin=_entity.origin;
	const int x=floor(origin.x / tile_w),
		y=floor(origin.y / tile_h),
		right=floor( (origin.x+_entity.w) / tile_w),
		top=floor( (origin.y+_entity.h) / tile_h);

#ifdef IS_DEBUG_BUILD
	if(debug_enabled && nullptr!=logger) {

		lm::log(*logger).debug()<<"tiles_in_box::find limits: x:"<<x<<" to "<<right<<" y:"<<y<<" to "<<top<<"\n";
	}
#endif

	//Scan and add.
	for(int cx=x; cx <= right; cx++) {

		for(int cy=y; cy <= top; cy++) {

			if(_finder.has(cx, cy)) {

#ifdef IS_DEBUG_BUILD

				if(debug_enabled && nullptr!=logger) {

					lm::log(*logger).debug()<<"will add tile in "<<cx<<","<<cy<<"\n";
				}
#endif
				result.push_back(_finder.get(cx, cy));
			}
		}
	}

	return result;
}
