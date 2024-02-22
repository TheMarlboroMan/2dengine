#pragma once

#include <d2d/collision/motion_axis_limiter.h>
#include <iostream>

namespace app {

class ladder:
	public d2d::collision::motion_axis_limiter {

	public:

	                                        ladder(int, int, int);
};

std::ostream& operator<<(std::ostream&, const ladder&);
}
