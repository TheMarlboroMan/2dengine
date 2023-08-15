#pragma once

#include <ldv/camera.h>
#include <ldv/screen.h>
#include <ldv/color.h>
#include "d2d/collision/spatiable.h"
#include <vector>

namespace d2d { namespace video {

/**
 * a ready-made class to draw stuff in debug mode-
 */
class debug_display {

	public:

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

	ldv::camera					camera; 

	private:

	ldv::rgba_color				bgcolor;

};

}}
