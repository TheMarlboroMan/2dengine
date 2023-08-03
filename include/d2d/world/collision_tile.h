#pragma once

#include "d2d/collision/spatiable.h"
#include "d2d/collision/shaper.h"
#include "d2d/world/collision_tile_implementation.h"

#include <ostream>
#include <stdexcept>

namespace d2d { namespace world {

struct collision_tile:
	public collision::spatiable {

	collision_tile(int, int, int, d2d::collision::shaper&, const d2d::world::collision_tile_implementation&);
/**
 *TODO; This should not be here!!
	enum types {
		full=1,
		half_bottom=2,
		half_top=3,
		half_left=4,
		half_right=5,
		quarter_bottom_left=6,
		quarter_bottom_right=7,
		quarter_top_left=8,
		quarter_top_right=9
	};
*/

	int                     x,
	                        y,
	                        type;
	//!The implementation of the shaper is some sort of flyweight thingy, 
	//!allowed to change. A pointer, because we should be able to copy, assign
	//!and do vector stuff to this class.
	d2d::collision::shaper *           shaper;

	//Same, another pointer. Cannot be null!!
	d2d::world::collision_tile_implementation const * const cimpl;

	//begin implementation of spatiable
	virtual collision::box&            get_box() {return shaper->get_box(cimpl->to_shape(*this), x, y);}
	virtual const collision::box&      get_box() const {return shaper->get_box(cimpl->to_shape(*this), x, y);}
	virtual const collision::box&      get_previous_box() const {return shaper->get_box(cimpl->to_shape(*this), x, y);}
	virtual collision::point&          get_origin() {return shaper->get_box(cimpl->to_shape(*this), x, y).origin;}
	virtual const collision::point&    get_origin() const {return shaper->get_box(cimpl->to_shape(*this), x, y).origin;}
	virtual double          get_x() const {return shaper->get_box(cimpl->to_shape(*this), x, y).origin.x;}
	virtual double          get_y() const {return shaper->get_box(cimpl->to_shape(*this), x, y).origin.y;}
	virtual int             get_w() const {return shaper->get_box(cimpl->to_shape(*this), x, y).w;}
	virtual int             get_h() const {return shaper->get_box(cimpl->to_shape(*this), x, y).h;}
	virtual color           get_debug_outline_color() const {return cimpl->get_outline_color(*this);}
	virtual color           get_debug_fill_color() const {return cimpl->get_fill_color(*this);}
	//end implementation of spatiable

	friend std::ostream&    operator<<(std::ostream&, const collision_tile&);

	private:
};

std::ostream& operator<<(std::ostream&, const collision_tile&);
}}
