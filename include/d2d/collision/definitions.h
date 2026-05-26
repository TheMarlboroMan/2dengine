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

using t_position=double; //type of x and y, also type of vectors and rays.
using t_size=int; //type of w and h

using point=ldt::point_2d<t_position>;
using fixed_point=ldt::point_2d<int>;
using box=ldt::box<t_position, t_size>;
using motion_vector=ldt::vector_2d<t_position>;
using ray=ldt::segment_2d<t_position>;
enum class box_edge { top, right, bottom ,left };
enum aabb_edges {top=1, bottom=2, right=4, left=8};

}}
