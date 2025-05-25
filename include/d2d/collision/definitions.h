#pragma once

#include <ldt/box.h>
#include <ldt/vector_2d.h>
#include <ldt/segment_2d.h>

namespace d2d { namespace collision {
 
/**
 * the simplest structure to represent tile coordinates.
*/
struct tile_coords{
	int x{0}, y{0};
};

using point=ldt::point_2d<double>;
using fixed_point=ldt::point_2d<int>;
using box=ldt::box<double, int>;
using motion_vector=ldt::vector_2d<double>;
using ray=ldt::segment_2d<double>;

}}
