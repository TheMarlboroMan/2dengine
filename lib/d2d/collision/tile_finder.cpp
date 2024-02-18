#include <d2d/collision/tile_finder.h>

using namespace d2d::collision;

std::ostream& d2d::collision::operator<<(
	std::ostream& _stream,
	const tile_finder::limits& _limit
) {

	_stream<<"left: "<<_limit.left<<" right:"<<_limit.right
		<<" bottom: "<<_limit.bottom<<" top:"<<_limit.top
		<<" size: "<<_limit.w<<","<<_limit.h;
	return _stream;
}

