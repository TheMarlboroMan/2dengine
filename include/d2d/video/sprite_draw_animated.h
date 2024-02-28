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
 * draws a single animation frame.
 */
	void            draw_frame(ldv::screen&, const ldv::camera&, ldv::point, const ldtools::animation&, int);
	void            draw_frame(ldv::screen&, const ldv::camera&, ldv::point, int, int);
//TODO: MIssing no camera, rect, with index and shit.

/**
 * the methods with a float can use an external timer.
 */
	void            draw(ldv::screen&, const ldv::camera&, ldv::point, const ldtools::animation&, float, sprite_draw::flags);
	void            draw(ldv::screen&, const ldv::camera&, ldv::point, const ldtools::animation&, float);
	void            draw(ldv::screen&, const ldv::camera&, ldv::point, const ldtools::animation&, sprite_draw::flags);
	void            draw(ldv::screen&, const ldv::camera&, ldv::point, const ldtools::animation&);
	void            draw(ldv::screen&, const ldv::camera&, ldv::point, int, float, sprite_draw::flags);
	void            draw(ldv::screen&, const ldv::camera&, ldv::point, int, float);
	void            draw(ldv::screen&, const ldv::camera&, ldv::point, int, sprite_draw::flags);
	void            draw(ldv::screen&, const ldv::camera&, ldv::point, int);
	void            draw(ldv::screen&, const ldv::camera&, ldv::rect, const ldtools::animation&, float, sprite_draw::flags);
	void            draw(ldv::screen&, const ldv::camera&, ldv::rect, const ldtools::animation&, float);
	void            draw(ldv::screen&, const ldv::camera&, ldv::rect, const ldtools::animation&, sprite_draw::flags);
	void            draw(ldv::screen&, const ldv::camera&, ldv::rect, const ldtools::animation&);
	void            draw(ldv::screen&, const ldv::camera&, ldv::rect, int, float, sprite_draw::flags);
	void            draw(ldv::screen&, const ldv::camera&, ldv::rect, int, float);
	void            draw(ldv::screen&, const ldv::camera&, ldv::rect, int, sprite_draw::flags);
	void            draw(ldv::screen&, const ldv::camera&, ldv::rect, int);


/** same as above, no camera */
	void            draw(ldv::screen&, ldv::point, const ldtools::animation&, float, sprite_draw::flags);
	void            draw(ldv::screen&, ldv::point, const ldtools::animation&, float);
	void            draw(ldv::screen&, ldv::point, const ldtools::animation&, sprite_draw::flags);
	void            draw(ldv::screen&, ldv::point, const ldtools::animation&);
	void            draw(ldv::screen&, ldv::point, int, float, sprite_draw::flags);
	void            draw(ldv::screen&, ldv::point, int, float);
	void            draw(ldv::screen&, ldv::point, int, sprite_draw::flags);
	void            draw(ldv::screen&, ldv::point, int);
	void            draw(ldv::screen&, ldv::rect, const ldtools::animation&, float, sprite_draw::flags);
	void            draw(ldv::screen&, ldv::rect, const ldtools::animation&, float);
	void            draw(ldv::screen&, ldv::rect, const ldtools::animation&, sprite_draw::flags);
	void            draw(ldv::screen&, ldv::rect, const ldtools::animation&);
	void            draw(ldv::screen&, ldv::rect, int, float, sprite_draw::flags);
	void            draw(ldv::screen&, ldv::rect, int, float);
	void            draw(ldv::screen&, ldv::rect, int, sprite_draw::flags);
	void            draw(ldv::screen&, ldv::rect, int);

	private:

	const ldtools::animation_table * animation_table{nullptr};
	float                           internal_timer{0.f};
};
}}
