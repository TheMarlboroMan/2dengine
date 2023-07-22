#pragma once

#include "d2d/collision/spatiable.h"
#include "d2d/collision/shaper.h"

#include <ostream>
#include <stdexcept>

namespace d2d { namespace world {

struct collision_tile:
	public collision::spatiable {

	collision_tile(int, int, int, d2d::collision::shaper&);

	int                     x,
	                        y,
	                        type;
	//!The implementation of the shaper is some sort of flyweight thingy, 
	//!allowed to change. A pointer, because we should be able to copy, assign
	//!and do vector stuff to this class.
	d2d::collision::shaper *           shaper;

	//begin implementation of spatiable
	virtual collision::box&            get_box() {return shaper->get_box(type, x, y);}
	virtual const collision::box&      get_box() const {return shaper->get_box(type, x, y);}
	virtual const collision::box&      get_previous_box() const {return shaper->get_box(type, x, y);}
	virtual collision::point&          get_origin() {return shaper->get_box(type, x, y).origin;}
	virtual const collision::point&    get_origin() const {return shaper->get_box(type, x, y).origin;}
	virtual double          get_x() const {return shaper->get_box(type, x, y).origin.x;}
	virtual double          get_y() const {return shaper->get_box(type, x, y).origin.y;}
	virtual int             get_w() const {return shaper->get_box(type, x, y).w;}
	virtual int             get_h() const {return shaper->get_box(type, x, y).h;}
	//end implementation of spatiable

	friend std::ostream&    operator<<(std::ostream&, const collision_tile&);
};

std::ostream& operator<<(std::ostream&, const collision_tile&);
}}
