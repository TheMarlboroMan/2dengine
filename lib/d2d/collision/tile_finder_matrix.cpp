#include <d2d/collision/tile_finder_matrix.h>
#include <algorithm>
#include <iostream>

using namespace d2d::collision;

void tile_finder_matrix::load(
	const std::vector<tile>& _tiles
) {

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

	int min_x=x.first->x,
		max_x=x.second->x,
		min_y=y.first->y,
		max_y=y.second->y;

	int columns=max_x-min_x;
	int rows=max_y-min_y;
	int cells=columns*rows;

	std::cout<<columns<<"x"<<rows;
	data.reserve(cells);
	data.insert(std::begin(data), cells, nullptr);

	//TODO: how will the offset work?
	//
	//TODO: Fill our shit up xD.
}

void tile_finder_matrix::clear() {

	offset_x=0;
	offset_y=0;
	data.clear();
}

bool tile_finder_matrix::has(
	int _x,
	int _y
) const {

	//TODO: throw if out of bounds??

	//TODO: Calculate index.
	
	return false;
}

d2d::collision::tile * tile_finder_matrix::get(
	int _x,
	int _y
) const {

	//TODO: throw if out of bounds??
	//
	//TODO: Calculate index.
	//
	return nullptr;
}
