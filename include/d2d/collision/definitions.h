#pragma once

#include <ldt/box.h>
#include <ldt/vector_2d.h>

namespace d2d { namespace collision {

using point=ldt::point_2d<double>;
using fixed_point=ldt::point_2d<int>;
using box=ldt::box<double, int>;
using motion_vector=ldt::vector_2d<double>;

}}
