#pragma once

#include <ldv/camera.h>
#include <ldv/screen.h>
#include <ldv/color.h>
#include <ldv/box_representation.h>
#include "d2d/collision/spatiable.h"
#include <vector>

namespace d2d { namespace video {

/**
 * a ready-made class to draw stuff in debug mode, assuming a cartesian 
 * coordinate system. Assumes the origin of things is the bottom-left.
 * TODO: So far it assumes 
 */
class debug_display {

	public:

	//TODO: Provide a method to indicate where the origin lies (bottom left? center?...)
								debug_display(int, int);

/**
 * setups the background color
 */
	void						set_background_color(ldv::rgba_color _color) {bgcolor=_color;}

	void						clear(ldv::screen&);
/**
 *performs the drawing.
 */
	void						draw(ldv::screen&, const std::vector<d2d::collision::spatiable>&);
	void						draw(ldv::screen&, const d2d::collision::spatiable&);
	//TODO: Provide a variadic alternative.
/**
 *centers the debug camera on a given point. 
 */
	void						center_on(const d2d::collision::spatiable&);

/**
 * sets up the camera margin.
 */
	void                        set_center_margin(const ldv::rect& _rect) {camera.set_center_margin(_rect);}


	ldv::camera					camera; 

	private:

	ldv::rgba_color				bgcolor;
	ldv::box_representation     outline,
								fill;

};

}}
