#pragma once

#include "spatiable.h"
#include "d2d/motion/definitions.h"
#include <ostream>

namespace d2d { namespace collision {

/**
 * Very basic implementation of a spatiable will all bells and whistles, ready
 * to use and to be used in composition. All edges are solid by default.
 */
class spatiable_box:
	public d2d::collision::spatiable {

	public:

	                                    spatiable_box(t_position, t_position, t_size, t_size);
	                                    spatiable_box(d2d::collision::point, t_size, t_size);
	                                    spatiable_box(d2d::collision::box);
	void                                commit_box();
	void                                rollback_box();
	//!Expects the values as presented in d2d::collision::aabb_edge. Each "on" edge will be impassable.
	void                                set_solid_edges(int _edges) {edges=_edges;}

//Begin implementation of spatiable

	d2d::collision::box&                get_box() {return collision_box;}
	const d2d::collision::box&          get_box() const {return collision_box;}
	const d2d::collision::box&          get_previous_box() const {return previous_box;}
	d2d::collision::point&              get_origin() {return collision_box.origin;}
	const d2d::collision::point&        get_origin() const {return collision_box.origin;}
	t_position                          get_x() const {return collision_box.origin.x;}
	t_position                          get_y() const {return collision_box.origin.y;}
	t_size                              get_w() const {return collision_box.w;}
	t_size                              get_h() const {return collision_box.h;}
	bool                                is_passable_edge(d2d::collision::box_edge) const;
	d2d::motion::motion_vector          get_motion_vector() const {return vector;}
	void                                set_motion_vector(d2d::motion::motion_vector _v) {vector=_v;}
//End implementation of spatiable

	private:

	box                                 collision_box, 
	                                    previous_box; 
	motion_vector                       vector;
	int                                 edges{15}; //indicates solid edges.
};

std::ostream& operator<<(std::ostream&, const spatiable_box&);
}}
