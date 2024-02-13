#pragma once

#include <d2d/collision/spatiable.h>
#include <iostream>

namespace app {

class entity:
	public d2d::collision::spatiable {

	public:

	                                    entity(int, int);

//Begin implementation of spatiable

	virtual d2d::collision::box&             get_box() {return collision_box;}
	virtual const d2d::collision::box&       get_box() const {return collision_box;}
	virtual const d2d::collision::box&       get_previous_box() const {return collision_box;}
	virtual d2d::collision::point&           get_origin() {return collision_box.origin;}
	virtual const d2d::collision::point&     get_origin() const {return collision_box.origin;}
	virtual double                      get_x() const {return collision_box.origin.x;}
	virtual double                      get_y() const {return collision_box.origin.y;}
	virtual int                         get_w() const {return collision_box.w;}
	virtual int                         get_h() const {return collision_box.h;}
	virtual bool                            is_passable_edge(d2d::collision::box_edge) const {return false;}
	//virtual color                       get_debug_outline_color() const {return cimpl->get_outline_color(*this);}
	//virtual color                       get_debug_fill_color() const {return cimpl->get_fill_color(*this);}

//End implementation of spatiable

	private:

	d2d::collision::box                 collision_box;
};

std::ostream& operator<<(std::ostream&, const entity&);
}
