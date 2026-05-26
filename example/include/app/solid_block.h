#pragma once

#include <d2d/collision/spatiable.h>
#include <iostream>

namespace app {

class solid_block:
	public d2d::collision::spatiable {

	public:

	                                        solid_block(int, int, int, int);

//Begin implementation of spatiable

	virtual d2d::collision::box&            get_box() {return collision_box;}
	virtual const d2d::collision::box&      get_box() const {return collision_box;}
	virtual const d2d::collision::box&      get_previous_box() const {return collision_box;}
	virtual d2d::collision::point&          get_origin() {return collision_box.origin;}
	virtual const d2d::collision::point&    get_origin() const {return collision_box.origin;}
	virtual d2d::collision::t_position      get_x() const {return collision_box.origin.x;}
	virtual d2d::collision::t_position      get_y() const {return collision_box.origin.y;}
	virtual d2d::collision::t_size          get_w() const {return collision_box.w;}
	virtual d2d::collision::t_size          get_h() const {return collision_box.h;}
	virtual bool                            is_passable_edge(d2d::collision::box_edge) const {return false;}
	virtual d2d::collision::color           get_debug_outline_color() const {return ldv::rgba8(255,0,64,255);}
	virtual d2d::collision::color           get_debug_fill_color() const {return ldv::rgba8(128, 0, 64, 255);}


//End implementation of spatiable

	private:

	d2d::collision::box                 collision_box;
};

std::ostream& operator<<(std::ostream&, const solid_block&);
}
