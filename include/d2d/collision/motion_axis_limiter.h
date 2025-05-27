#pragma once

#include "spatiable.h"
#include <ostream>

namespace d2d { namespace collision {

/**
 * a box that can be used to constraint movement of a spatiable withín it,
 * allowing movement only in a single axis, like old school ladders or
 * lode runner ropes.
 */
class motion_axis_limiter
	:public spatiable {

	public:

	enum class axes{x, y};
/***
 * how will a spatiable align along the blocked axis? near=left/bottom to 
 * left/bottom of target, nearer * zero, far=right/top to right/top of target.
*/
	enum class alignments{near, center, far};

	                        motion_axis_limiter(const d2d::collision::box&, axes, alignments);
/**
 * limits spatiable position within the box of this limiter along the axis
 * specified by it. The other axis will be positioned according to the 
 * alignment of the limiter. The spatiable will have its movement limited in 
 * the axis to the size of this limiter.
 */
	void                    apply(spatiable&) const;
	motion_axis_limiter&    set_axis(axes);
	motion_axis_limiter&    set_alignment(alignments);

	axes                    get_axis() const {return axis;}
	alignments              get_alignment() const {return alignment;}

//Begin implementation of spatiable

	virtual d2d::collision::box&            get_box() {return collision_box;}
	virtual const d2d::collision::box&      get_box() const {return collision_box;}
	virtual const d2d::collision::box&      get_previous_box() const {return collision_box;}
	virtual d2d::collision::point&          get_origin() {return collision_box.origin;}
	virtual const d2d::collision::point&    get_origin() const {return collision_box.origin;}
	virtual double                          get_x() const {return collision_box.origin.x;}
	virtual double                          get_y() const {return collision_box.origin.y;}
	virtual int                             get_w() const {return collision_box.w;}
	virtual int                             get_h() const {return collision_box.h;}
	virtual bool                            is_passable_edge(d2d::collision::box_edge) const {return true;}
	virtual color                           get_debug_outline_color() const {return color{0, 0, 0, 255};}
	virtual color                           get_debug_fill_color() const {return color{128,128,128, 64};}
	virtual d2d::motion::motion_vector      get_motion_vector() const {return {0, 0};}
	virtual void                            set_motion_vector(d2d::motion::motion_vector) {}
	virtual void                            commit_box() {};
	virtual void                            rollback_box() {};

//End implementation of spatiable

	private:

	d2d::collision::box     collision_box;
	axes                    axis;
	alignments              alignment;
};

std::ostream& operator<<(std::ostream&, const motion_axis_limiter::axes&);
std::ostream& operator<<(std::ostream&, const motion_axis_limiter::alignments&);
std::ostream& operator<<(std::ostream&, const motion_axis_limiter&);
}}
