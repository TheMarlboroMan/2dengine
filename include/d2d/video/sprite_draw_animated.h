#pragma once

#include "sprite_draw.h"
#include <ldtools/animation_table.h>

namespace d2d { namespace video {

class sprite_draw_animated {

	public:

	                                sprite_draw_animated(
										const ldtools::sprite_table&, 
										const ldv::texture&,
										const ldtools::animation_table&
									);

	sprite_draw                     spr_draw;

/**
 * the timer must be fed from the outside.
 */
	void                            tic(float);

/**
 * replaces the current animation table.
 */
	sprite_draw_animated&           reset_animation(const ldtools::animation_table&);

/**
 * These methods use the internal timer.
 */
	void            draw(ldv::screen&, const ldv::camera&, ldv::point, const ldtools::animation&);
	void            draw(ldv::screen&, ldv::point, const ldtools::animation&);

/**
 * draw versions to be called with an outside timer.
 */
	void            draw(ldv::screen&, const ldv::camera&, ldv::point, const ldtools::animation&, float);
	void            draw(ldv::screen&, ldv::point, const ldtools::animation&, float);

	private:

	const ldtools::animation_table * animation_table{nullptr};
	float                           internal_timer{0.f};
};
}}
