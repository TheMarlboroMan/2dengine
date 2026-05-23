#pragma once

#include "definitions.h"
#include "../motion/definitions.h"
#include <ldv/color.h>
#include <ostream>


namespace d2d { namespace collision {

using color=ldv::rgba_color;

std::ostream& operator<<(std::ostream&, const box_edge&);

/**
*a spatiable is something that exists in a cartesian space that as an
* aabb bounding box. It also has "motion" capabilities because, why collisions
* if there is no motion? In any case, because not all spatiables will be
* able to move, there's only a virtual copy getter for the vector and virtual 
* setter that can be defined as having no effect.
*/
struct spatiable {

/**
*must return a color with which to draw the outline in debug mode
*/
	virtual color           get_debug_outline_color() const {return ldv::rgba8(32,32,32,255);}
/**
*must return a color with which to fill the outline in debug mode
*/
	virtual color           get_debug_fill_color() const {return ldv::rgba8(128, 128, 128, 255);}

	virtual                 ~spatiable()=0;

	/**
	* Must return a reference to the bounding box.
	*/
	virtual box&            get_box()=0;

	/**
	* Must return a reference to the bounding box.
	*/
	virtual const box&      get_box() const=0;

	/**
	* Must return a reference to the previous position bounding box.
	*/
	virtual const box&      get_previous_box() const=0;

	/**
    *Must make the previous box be a copy of the current box.
    */
	virtual void            commit_box()=0;

	/**
    *Must make the current previous box be a copy of the current box.
    */
	virtual void            rollback_box()=0;

	/**
	* Must return a reference to the boinding box origin point.
	*/
	virtual point&          get_origin()=0;

	/**
	* Must return a reference to the boinding box origin point.
	*/
	virtual const point&    get_origin() const=0;

	/**
	* Must return the x component of the origin. Exists only to provide a way
	* to directly access this property without passing through the box/origin.
	*/
	virtual double          get_x() const=0;

	/**
	* Must return the y component of the origin. Exists only to provide a way
	* to directly access this property without passing through the box/origin.
	*/
	virtual double          get_y() const=0;

	/**
	* Must return the width of the bounding box. Exists only to provide a way
	* to directly access this property without passing through the box getter.
	*/
	virtual int             get_w() const=0;

	/**
	* Must return the height of the bounding box. Exists only to provide a way
	* to directly access this property without passing through the box getter.
	*/
	virtual int             get_h() const=0;

	/**
	* Must return if the box is passable from the given edge.
	*/
	virtual bool            is_passable_edge(box_edge) const=0;

	/**
    * Must return a copy of the motion vector
    */
	virtual d2d::motion::motion_vector get_motion_vector() const=0;

	double                  get_motion_vector_x() const {return get_motion_vector().x;}
	double                  get_motion_vector_y() const {return get_motion_vector().y;}

	virtual void            set_motion_vector(d2d::motion::motion_vector)=0;

	void                    set_motion_vector_x(double _val) {

		auto vec=get_motion_vector();
		set_motion_vector({_val, vec.y});
	}

	void                    set_motion_vector_y(double _val) {

		auto vec=get_motion_vector();
		set_motion_vector({vec.x, _val});
	}

	/**
	* Returns the y component plus the box height.
	*/
	double                  get_top() const {return get_y()+get_h();}

	/**
	* Returns the x component plus the box width.
	*/
	double                  get_right() const {return get_x()+get_w();}
	/**
    * Returns true if boxes are different.
    */
	bool                    has_moved() const {return get_origin() != get_previous_box().origin;}

	/**
	* Replaces the bounding box with the given one.
	*/
	void                    set_box(box);

	/**
	* Replaces the box origin with the given one.
	*/
	void                    set_origin(point);

	/**
	* Displaces the box origin by the given point.
	*/
	void                    move_by(point);

	/**
	* Replaces the box origin x component with the given value.
	*/
	void                    set_x(double);

	/**
	* Replaces the box origin y component with the given value.
	*/
	void                    set_y(double);

	/**
	* Sets the bounding box width.
	*/
	void                    set_w(int);

	/**
	* Sets the bounding box height.
	*/
	void                    set_h(int);
};

}}
