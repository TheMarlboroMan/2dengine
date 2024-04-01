#pragma once

#include "spatiable.h"
#include "shaper.h"
#include "tile_implementation.h"

#include <ostream>
#include <stdexcept>

namespace d2d { namespace collision {

struct tile:
	public collision::spatiable {

	tile(int, int, int, d2d::collision::shaper&, const d2d::collision::tile_implementation&);

/**
 * Fact of the matter is that we may need to copy tiles and the const members
 * don't like defaults, so here we have some more constructors and operators.
 */
	tile(const tile&);
	tile& operator=(const tile&);

	int                     x,
	                        y,
	                        type;
	//!The implementation of the shaper is some sort of flyweight thingy, 
	//!allowed to change. A pointer, because we should be able to copy, assign
	//!and do vector stuff to this class.
	d2d::collision::shaper *           shaper;

	//Same, another pointer. Cannot be null!!
	d2d::collision::tile_implementation const * const cimpl;

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
	virtual bool            is_passable_edge(box_edge _edge) const {return cimpl->is_passable_edge(*this, _edge);}
	virtual color           get_debug_outline_color() const {return cimpl->get_outline_color(*this);}
	virtual color           get_debug_fill_color() const {return cimpl->get_fill_color(*this);}
	//end implementation of spatiable

	friend std::ostream&    operator<<(std::ostream&, const tile&);

	private:
};

std::ostream& operator<<(std::ostream&, const tile&);
}}
