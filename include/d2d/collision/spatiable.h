#pragma once

#include "definitions.h"
#include <ldv/color.h>

namespace d2d { namespace collision {

using color=ldv::rgba_color;

struct spatiable {

/**
*must return a color with which to draw the outline in debug mode
*/
	virtual color           get_debug_outline_color() const {return ldv::rgba8(0,0,0,0);}
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
	* Returns the y component plus the box height.
	*/
	double                  get_top() const {return get_y()+get_h();}

	/**
	* Returns the x component plus the box width.
	*/
	double                  get_right() const {return get_x()+get_w();}

	/**
	* Replaces the bounding box with the given one.
	*/
	void                    set_box(box);

	/**
	* Replaces the box origin with the given one.
	*/
	void                    set_origin(point);

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
