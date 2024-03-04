#include "d2d/collision/tile_limits.h"
#include <algorithm>

using namespace d2d::collision;

tile_limits::operator bool() const {

	return left || right || bottom || top || w || h;
}

tile_limits tile_limits_finder::find_limits(
	const std::vector<d2d::collision::tile>& _tiles
) const {

	if(!_tiles.size()) {

		return {0,0,0,0,0,0};
	}

	//find the smallest and largest x and y.
	auto x=std::minmax_element(
		std::begin(_tiles),
		std::end(_tiles),
		[](const d2d::collision::tile& _a, const d2d::collision::tile& _b) {
			return _a.x < _b.x;
		}
	);

	auto y=std::minmax_element(
		std::begin(_tiles),
		std::end(_tiles),
		[](const d2d::collision::tile& _a, const d2d::collision::tile& _b) {
			return _a.y < _b.y;
		}
	);

	//Plus one because the smallest map is 1x1 thus maxx=1 minx=1 1-1=0+1 => 1.
	return {
		x.first->x, 
		x.second->x, 
		y.first->y, 
		y.second->y, 
		x.second->x-x.first->x+1,
		y.second->y-y.first->y+1
	};
}

std::ostream& d2d::collision::operator<<(
	std::ostream& _stream,
	const tile_limits& _limit
) {

	_stream<<"left: "<<_limit.left<<" right:"<<_limit.right
		<<" bottom: "<<_limit.bottom<<" top:"<<_limit.top
		<<" size: "<<_limit.w<<","<<_limit.h;
	return _stream;
}
