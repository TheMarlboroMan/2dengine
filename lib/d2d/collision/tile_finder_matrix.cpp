#include "d2d/collision/tile_finder_matrix.h"
#include "d2d/collision/tile_limits.h"
#include "d2d/collision/exception.h"
#include <algorithm>
#include <iostream>

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

		throw exception("cannot load empty map");
	}

	d2d::collision::tile_limits_finder tlf{};
	limit=tlf.find_limits(_tiles);

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

tile_coords tile_finder_matrix::to_tile_coords(
	d2d::collision::point _point
)const {

	int x=floor(_point.x/tile_w);
	int y=floor(_point.y/tile_h);

	return {x, y};
}

d2d::collision::point tile_finder_matrix::to_point(
	tile_coords _coord
)const {

	double x=_coord.x*tile_w;
	double y=_coord.y*tile_h;
	return {x, y};
}

bool tile_finder_matrix::has(
	tile_coords _point
) const {

	if(
		_point.x < limit.left || _point.x > limit.right
		 || _point.y < limit.bottom || _point.y > limit.top
	) {

		return false;
	}

	int x=_point.x+offset_x,
	    y=_point.y+offset_y;

	int index=x + (y * limit.w);
	return nullptr!=data.at(index);
}

const d2d::collision::tile * tile_finder_matrix::get(
	tile_coords _point
) const {

	if(
		_point.x < limit.left || _point.x > limit.right
		|| _point.y < limit.bottom || _point.y > limit.top
	) {

		return nullptr;
	}

	int x=_point.x+offset_x,
	    y=_point.y+offset_y;

	int index=x + (y * limit.w);
	return data.at(index);
}

d2d::collision::tile * tile_finder_matrix::get(
	tile_coords _point
) {

	if(
		_point.x < limit.left || _point.x > limit.right
		|| _point.y < limit.bottom || _point.y > limit.top
	) {

		return nullptr;
	}

	int x=_point.x+offset_x,
	    y=_point.y+offset_y;

	int index=x + (y * limit.w);
	return data.at(index);
}
