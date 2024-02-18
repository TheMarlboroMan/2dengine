#include <d2d/collision/tile_finder_matrix.h>
#include <algorithm>
#include <iostream>
#include <stdexcept>

using namespace d2d::collision;

tile_finder_matrix::tile_finder_matrix(
	int _w,
	int _h
):
	tile_w{_w},
	tile_h{_h}
{}

void tile_finder_matrix::load(
	const std::vector<tile>& _tiles
) {

	if(!_tiles.size()) {

		throw std::runtime_error("cannot load empty map");
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
	limit={
		x.first->x, x.second->x, 
		y.first->y, y.second->y, 
		x.second->x-x.first->x+1,
		y.second->y-y.first->y+1
	};
	int cell_count=limit.w*limit.h;

	//fill the data struture will null pointers.
	data.reserve(cell_count);
	data.insert(std::begin(data), cell_count, nullptr);

/**
Tiles are stored normalized, 0,0 would be the bottom-left part
so we deal only with positive indexes.
there will be some sort of mapping here using an offset. The most negative
tile must be compensated so its coordinate is zero. Considering the lefmost
tile as x=-5 it must be considered zero, and a tile in x=7 must be considered 
7+5 = 12. In the same vein, if the leftmost tile is x=5, a tile in x=7 must
be considered as 7-5=2. Thus, the offset is just the negative value of
the leftmost tile.
The same goes for the y axis.
*/

	offset_x=-limit.left;;
	offset_y=-limit.bottom;

	for(const auto& tile : _tiles) {

		int tile_x=(tile.get_x() / tile_w)+offset_x;
		int tile_y=(tile.get_y() / tile_h)+offset_y;

		int index=tile_x + (tile_y * limit.w);
		data[index]=const_cast<d2d::collision::tile*> (&tile);
	}
}

void tile_finder_matrix::clear() {

	offset_x=0;
	offset_y=0;
	limit={0,0,0,0,0,0};
	data.clear();
}

bool tile_finder_matrix::has(
	int _x,
	int _y
) const {

	if(
		_x < limit.left || _x > limit.right
		 || _y < limit.bottom || _y > limit.top
	) {

		return false;
	}

	int x=_x+offset_x,
	    y=_y+offset_y;

	int index=x + (y * limit.w);
	return nullptr!=data.at(index);
}

const d2d::collision::tile * tile_finder_matrix::get(
	int _x,
	int _y
) const {

	if(
		_x < limit.left || _x > limit.right
		|| _y < limit.bottom || _y > limit.top
	) {

		return nullptr;
	}

	int x=_x+offset_x,
	    y=_y+offset_y;

	int index=x + (y * limit.w);
	return data.at(index);
}

d2d::collision::tile * tile_finder_matrix::get(
	int _x,
	int _y
) {

	if(
		_x < limit.left || _x > limit.right
		|| _y < limit.bottom || _y > limit.top
	) {

		return nullptr;
	}

	int x=_x+offset_x,
	    y=_y+offset_y;

	int index=x + (y * limit.w);
	return data.at(index);
}
